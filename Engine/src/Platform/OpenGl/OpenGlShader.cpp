#include <GL/glew.h>
#include "OpenGlShader.hpp"

#include <Utilities/InstrumentationMacros.hpp>
#include <Utilities/Timer.hpp>
#include <Core/Utils.hpp>

#include <filesystem>
#include <fstream>
#include <functional>
#include <Core/Core.hpp>
#include <sstream>

#include <nlohmann/json.hpp>
#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>
#include <spirv_reflect.hpp>

namespace ant::OpenGl
{

    class OpenGlShaderUtils
    {
    public:
        static const char *GetOpenGlCacheFileExtension(GLenum stage)
        {
            if (stage == GL_VERTEX_SHADER)
                return ".vertglshader";
            if (stage == GL_FRAGMENT_SHADER)
                return ".pixelglshader";

            CORE_ASSERT(false, "Unsupported shader stage");
            return "FAIL";
        }

        static std::filesystem::path GetOpenGlCachePath(const std::string &name, GLenum stage)
        {
            std::filesystem::path path = Shader::Utils::GetCacheDirectory();
            path.append(name + GetOpenGlCacheFileExtension(stage));
            return path;
        }

        static bool HasCacheFile(const std::filesystem::path &path)
        {
            return std::filesystem::exists(path) && std::filesystem::file_size(path);
        }

        static shaderc_target_env RenderApiEnumToShaderc(RenderApi api)
        {
            if (api == RenderApi::OpenGl)
                return shaderc_target_env_opengl;
            if (api == RenderApi::Vulcan)
                return shaderc_target_env_vulkan;

            CORE_ASSERT(false, "unsupported target/api");
            return shaderc_target_env_default;
        }

        static shaderc_shader_kind ShaderStageToShadercKind(Shader::Stage stage)
        {
            if (stage == Shader::Stage::Vertex)
                return shaderc_vertex_shader;
            if (stage == Shader::Stage::Fragment)
                return shaderc_fragment_shader;

            CORE_ASSERT(false, "Unsupported shader stage");
            return shaderc_miss_shader;
        }

        static GLenum ShaderStageToGl(Shader::Stage stage)
        {
            CORE_ASSERT(stage != Shader::Stage::Invalid, "Invalid stage");

            switch (stage)
            {
            case Shader::Stage::Fragment:
                return GL_FRAGMENT_SHADER;
                break;

            case Shader::Stage::Vertex:
                return GL_VERTEX_SHADER;
                break;

            default:
                break;
            }

            CORE_ASSERT(false, "Not implemented stage conversion");
            return GL_INVALID_ENUM;
        }
    };

    GlShader::GlShader(bool trackSource, bool useTextShaders)
        : m_useTextShaders(useTextShaders), Shader(trackSource)
    {
        CORE_DETAILED_PROFILE_FUNC();
        m_glProgram = GL_INVALID_INDEX;
        m_glProgram = glCreateProgram();

        CORE_ASSERT(m_glProgram != GL_INVALID_INDEX, "Failed do create glProgram (GlShader)");
    }

    GlShader::~GlShader()
    {
        CORE_DETAILED_PROFILE_FUNC();
        glDeleteProgram(m_glProgram);
    }

    void GlShader::OnInit()
    {

        CORE_DETAILED_PROFILE_FUNC();
        CORE_ASSERT(dm_SpirvBinaries->size() != 0, "no compiled spirv");

        if (!m_useTextShaders)
            m_openglBinaries = ant::MakeScope<StageMap<std::vector<uint32_t>>>();

        std::vector<GLenum> componentIds;
        for (auto &[stage, vbinarry] : *dm_SpirvBinaries)
        {

            GLuint shaderId = glCreateShader(OpenGlShaderUtils::ShaderStageToGl(stage));
            componentIds.push_back(shaderId);

            if (m_useTextShaders)
            {
                spirv_cross::CompilerGLSL glslCompiler(dm_SpirvBinaries->at(stage));
                std::string source = glslCompiler.compile();

                const GLchar *codeArray[] = {source.c_str()};
                glShaderSource(shaderId, 1, codeArray, NULL);
                glCompileShader(shaderId);
            }
            else
            {
                GetOpenGlBinary(stage, dm_recompilePlatformShaders.at(stage));
                std::vector<uint32_t> &binary = m_openglBinaries->at(stage);

                glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, binary.data(), binary.size() * sizeof(uint32_t));
                glSpecializeShaderARB(shaderId, "main", 0, 0, 0);
            }

            int res;
            glGetObjectParameterivARB(shaderId, GL_COMPILE_STATUS, &res);

            if (res == GL_FALSE)
            {
                std::stringstream ss;
                int len;
                glGetObjectParameterivARB(shaderId, GL_INFO_LOG_LENGTH, &len);
                char *mes = (char *)alloca(len);
                glGetShaderInfoLog(shaderId, len, &len, mes);
                ss << Convert::StageEnumToStageString(stage) << " GlShader injection failed! " << mes;
                for (auto id : componentIds)
                    glDeleteShader(id);
                CORE_ASSERT(false, ss.str());
            }

            glAttachShader(m_glProgram, shaderId);
        }

        glLinkProgram(m_glProgram);
        glValidateProgram(m_glProgram);

        for (auto id : componentIds)
            glDeleteShader(id);

        m_openglBinaries.reset(); // clear buffers
    }

    void GlShader::Bind() const
    {
        CORE_ASSERT(dm_specifications.size() != 0, "No shader specification! Are you missing Init() call?");
        CORE_DETAILED_PROFILE_FUNC();
        glUseProgram(m_glProgram);
    }

    void GlShader::UnBind() const
    {
        CORE_DETAILED_PROFILE_FUNC();
        glUseProgram(0);
    }

    void GlShader::GetOpenGlBinary(Shader::Stage stage, bool forceRecompilation)
    {
        CORE_DETAILED_PROFILE_FUNC();

        (*m_openglBinaries)[stage].clear();

        const std::filesystem::path &path = OpenGlShaderUtils::GetOpenGlCachePath(dm_name, OpenGlShaderUtils::ShaderStageToGl(stage));

        if (OpenGlShaderUtils::HasCacheFile(path) && !forceRecompilation)
        {
            ant::Utils::LoadBinary(path, &m_openglBinaries->at(stage));
        }
        else
        {
            spirv_cross::CompilerGLSL glslCompiler(dm_SpirvBinaries->at(stage));
            std::string c = glslCompiler.compile();
            SpirvCompileOpenGL((*m_openglBinaries)[stage], c, stage, true);
            ant::Utils::SaveBinary(path, &m_openglBinaries->at(stage));
        }
    }

    void GlShader::SpirvCompileOpenGL(std::vector<uint32_t> &target, const std::string &source, Shader::Stage stage, bool optimize)
    {
        CORE_DETAILED_PROFILE_FUNC();
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

        shaderc_optimization_level optimization = optimize ? shaderc_optimization_level_performance : shaderc_optimization_level_zero;
        options.SetOptimizationLevel(optimization);

        shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, OpenGlShaderUtils::ShaderStageToShadercKind(stage), dm_filePath.string().c_str(), options);
        CORE_ASSERT(result.GetCompilationStatus() == shaderc_compilation_status_success, "Shader {" + dm_name + "} failed to compile: " + result.GetErrorMessage());

        target = std::vector<uint32_t>(result.begin(), result.end());
    }

}