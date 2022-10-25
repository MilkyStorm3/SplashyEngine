#include "Shader.hpp"
#include <Utilities/Timer.hpp>
#include <Render/RendererCommands.hpp>
#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>
#include <spirv_reflect.hpp>
#include <Core/Utils.hpp>

namespace ant
{

    const char *Shader::Utils::GetCacheDirectory()
    {
        return "cache/shaders";
    }

    const char *Shader::Convert::StageEnumToStageString(Shader::Stage stage)
    {
        if (stage == Shader::Stage::Vertex)
            return "vertex";
        if (stage == Shader::Stage::Fragment)
            return "fragment";

        CORE_ASSERT(false, "Unsupported shader stage");
        return "none";
    }

    void Shader::Utils::CreateCacheDirectoryIfNeeded()
    {
        auto dir = GetCacheDirectory();
        if (!std::filesystem::exists(dir))
            std::filesystem::create_directories(dir);
    }

    const char *Shader::Utils::GetDescriptiorFileExtension(Shader::Stage stage)
    {
        if (stage == Stage::Vertex)
            return ".vertdesc.json";
        if (stage == Stage::Fragment)
            return ".pixeldesc.json";

        CORE_ASSERT(false, "Unsupported shader stage");
        return "FAIL";
    }

    class Lutils
    {
    public:
        static bool HasCacheFile(const std::filesystem::path &path)
        {
            return std::filesystem::exists(path) && std::filesystem::file_size(path);
        }

        static shaderc_shader_kind ShaderStageToShadercKind(Shader::Stage stage)
        {
            if (stage == Shader::Stage::Vertex)
                return shaderc_vertex_shader;
            if (stage == Shader::Shader::Fragment)
                return shaderc_fragment_shader;

            CORE_ASSERT(false, "Unsupported shader stage");
            return shaderc_miss_shader;
        }

        static std::filesystem::path GetVCachePath(const std::string &name, Shader::Stage stage)
        {
            std::filesystem::path path = Shader::Utils::GetCacheDirectory();
            path.append(name + GetVCacheFileExtension(stage));
            return path;
        }

        static const char *GetVCacheFileExtension(Shader::Stage stage)
        {
            if (stage == Shader::Stage::Vertex)
                return ".vertvshader";
            if (stage == Shader::Shader::Fragment)
                return ".pixelvshader";

            CORE_ASSERT(false, "Unsupported shader stage");
            return "FAIL";
        }
    };

    void Shader::Parse(const std::filesystem::path &filePath)
    {

        CORE_DETAILED_PROFILE_FUNC();
        std::ifstream file(filePath);
        Shader::Stage type = Shader::Stage::Invalid;
        std::string line;

        while (std::getline(file, line))
        {
            if (!line.compare("#vertexShader"))
            {
                type = Shader::Stage::Vertex;
                continue;
            }

            if (!line.compare("#fragmentShader"))
            {
                type = Shader::Stage::Fragment;
                continue;
            }

            line.push_back('\n');
            CORE_ASSERT(type != Shader::Stage::Invalid, "Check shader definitions");
            dm_sources[type].append(line);
            line.clear();
        }

        file.close();
    }

    void Shader::LoadFromFile(const std::filesystem::path &filePath)
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();

        CORE_ASSERT(!filePath.empty(), "GlShader path not provided!");
        CORE_ASSERT(std::filesystem::exists(filePath), "Cannot find GlShader file! " + filePath.string());

        auto fname = filePath.filename().string();
        dm_name = fname.substr(0, fname.find_last_of('.'));

        dm_filePath = filePath;

        Parse(filePath);
    }

    void Shader::FromSource(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        CORE_ASSERT(!name.empty(), "Name not provided!");
        CORE_ASSERT(!vertexSrc.empty(), "Vertex shader source not provided!");
        CORE_ASSERT(!fragmentSrc.empty(), "Fragment shader source not provided!");

        dm_name = name;
        dm_sources[Shader::Stage::Vertex] = vertexSrc;
        dm_sources[Shader::Stage::Fragment] = fragmentSrc;
    }

    bool Shader::LoadSpecification(Shader::Stage stage)
    {
        CORE_DETAILED_PROFILE_FUNC();
        std::filesystem::path path = Utils::GetCacheDirectory();
        path.append(dm_name + Utils::GetDescriptiorFileExtension(stage));

        if (Lutils::HasCacheFile(path))
        {
            std::ifstream file(path);
            CORE_ASSERT(file.is_open(), "cannot open file");
            size_t size = std::filesystem::file_size(path);

            std::string text;
            text.resize(size);
            file.read(text.data(), size);
            file.close();

            dm_specifications[stage] = nlohmann::json::parse(text);
            return true;
        }
        return false;
    }

    void Shader::Init()
    {

        CORE_INTERMEDIATE_PROFILE_FUNC();
        Timer timer;
        Utils::CreateCacheDirectoryIfNeeded();

        std::hash<std::string> hasher;

        for (auto &[stage, source] : dm_sources)
        {
            if (source.length() == 0)
            {
                std::stringstream msg;
                msg << "There has to be a " << Convert::StageEnumToStageString(stage) << " source!";
                CORE_ASSERT(false, msg.str().c_str());
            }

            if (dm_trackSourceFile)
            {
                bool recompile = true;
                if (LoadSpecification(stage))
                {
                    size_t sourceHash = hasher(source);
                    size_t previousHash = 0;
                    if (dm_specifications[stage].contains("hash"))
                        previousHash = dm_specifications[stage]["hash"];

                    recompile = sourceHash != previousHash;
                }

                if (recompile)
                    CreateSpecification(source, stage);

                LoadOrCompileVBinary(source, stage, recompile);
            }
            else
            {
                bool compile = !LoadSpecification(stage);

                if (compile)
                    CreateSpecification(source, stage);

                LoadOrCompileVBinary(source, stage, compile);
            }
        }

        OnInit();

        dm_sources.clear();
        dm_SpirvBinaries.clear();

        CORE_INFO("Shader [{0}] created in {1} ms", dm_name, timer.ElapsedMillis());
    }

    void Shader::CreateSpecification(const std::string &source, Shader::Stage stage)
    {
        CORE_DETAILED_PROFILE_FUNC();
        std::filesystem::path path = Utils::GetCacheDirectory();
        path.append(dm_name + Utils::GetDescriptiorFileExtension(stage));

        std::vector<uint32_t> binary;
        SpirvCompile(binary, source, stage, false);

        spirv_cross::CompilerReflection reflectionCompiler(binary);
        reflectionCompiler.set_format("json");
        dm_specifications[stage] = nlohmann::json::parse(reflectionCompiler.compile());

        if (dm_trackSourceFile)
        {
            std::hash<std::string> hasher;
            dm_specifications[stage]["hash"] = hasher(source);
        }

        std::string text = dm_specifications[stage].dump(4);
        std::ofstream file(path);
        CORE_ASSERT(file.is_open(), "cannot create file");
        file.write(text.data(), text.size());
    }

    void Shader::SpirvCompile(std::vector<uint32_t> &target, const std::string &source, Shader::Stage stage, bool optimize)
    {
        CORE_DETAILED_PROFILE_FUNC();
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

        shaderc_optimization_level optimization = optimize ? shaderc_optimization_level_performance : shaderc_optimization_level_zero;
        options.SetOptimizationLevel(optimization);

        shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, Lutils::ShaderStageToShadercKind(stage), dm_filePath.string().c_str(), options);
        CORE_ASSERT(result.GetCompilationStatus() == shaderc_compilation_status_success, "Shader {" + dm_name + "} failed to compile: " + result.GetErrorMessage());

        target = std::vector<uint32_t>(result.begin(), result.end());

        // TODO handle compiler errors
    }

    void Shader::LoadOrCompileVBinary(const std::string &source, Stage stage, bool recompile)
    {
        dm_SpirvBinaries[stage].clear();

        const std::filesystem::path &path = Lutils::GetVCachePath(dm_name, stage);

        if (Lutils::HasCacheFile(path) && !recompile)
        {
            ant::Utils::LoadBinary(path, &dm_SpirvBinaries[stage]);
        }
        else
        {
            SpirvCompile(dm_SpirvBinaries[stage], source, stage, true);
            ant::Utils::SaveBinary(path, &dm_SpirvBinaries[stage]);
        }
    }
}
