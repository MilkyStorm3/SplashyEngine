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

    const char *GlShader::GetOpenGlCacheFileExtension(GLenum stage)
    {
        if (stage == GL_VERTEX_SHADER)
            return ".vertglshader";
        if (stage == GL_FRAGMENT_SHADER)
            return ".pixelglshader";

        CORE_ASSERT(false, "Unsupported shader stage");
        return "FAIL";
    }

    std::filesystem::path GlShader::GetOpenGlCachePath(const std::string &name, GLenum stage)
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
        if (stage == Shader::Shader::Fragment)
            return shaderc_fragment_shader;

        CORE_ASSERT(false, "Unsupported shader stage");
        return shaderc_miss_shader;
    }

    GlShader::GlShader(bool trackSource, bool useTextShaders)
        : m_useTextShaders(useTextShaders)
    {
        dm_trackSourceFile = trackSource;
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
        if (m_useTextShaders)
        {
            ComposeGlProgramFromSource();
        }
        else
        {
            // cross compile to glspirv
            ComposeGlProgram();
        }
        // compose opengl binary
        // clear buffers
    }

    void GlShader::Bind() const
    {
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
        m_openglBinaries[stage].clear();

        const std::filesystem::path &path = GetOpenGlCachePath(dm_name, ShaderStageToGl(stage));

        if (HasCacheFile(path) && !forceRecompilation)
        {
            ant::Utils::LoadBinary(path, &m_openglBinaries[stage]);
        }
        else
        {
            spirv_cross::CompilerGLSL glslCompiler(dm_SpirvBinaries[stage]);
            std::string c = glslCompiler.compile();
            CORE_WARN("glsl source {0}", c.c_str());
            SpirvCompileOpenGL(m_openglBinaries[stage], c, stage, true);
            ant::Utils::SaveBinary(path, &m_openglBinaries[stage]);
        }
    }

    void GlShader::ComposeGlProgram()
    {
        CORE_DETAILED_PROFILE_FUNC();
        CORE_ASSERT(m_openglBinaries.size() > 0, "no opengl binary");
        std::vector<GLenum> componentIds;
        for (auto &[stage, bvinary] : dm_SpirvBinaries)
        {
            GetOpenGlBinary(stage, true); // It always recompiles binary, change that
            std::vector<uint32_t> &binary = m_openglBinaries[stage];

            GLuint shaderId = glCreateShader(ShaderStageToGl(stage));
            componentIds.push_back(shaderId);

            glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, binary.data(), binary.size() * sizeof(uint32_t));
            glSpecializeShaderARB(shaderId, "main", 0, 0, 0);

            int res;
            glGetObjectParameterivARB(shaderId, GL_COMPILE_STATUS, &res);

            if (res == GL_FALSE)
            {
                std::stringstream ss;
                int len;
                glGetObjectParameterivARB(shaderId, GL_INFO_LOG_LENGTH, &len);
                char *mes = (char *)alloca(len);
                glGetShaderInfoLog(shaderId, len, &len, mes);
                ss << Shader::Convert::StageEnumToStageString(stage) << " GlShader injection failed! " << mes;
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
    }

    void GlShader::ComposeGlProgramFromSource()
    {
        CORE_DETAILED_PROFILE_FUNC();
        CORE_ASSERT(dm_SpirvBinaries.size() > 0, "no shader source");

        std::vector<GLenum> componentIds;
        for (auto &[stage, vbinarry] : dm_SpirvBinaries)
        {
            spirv_cross::CompilerGLSL glslCompiler(dm_SpirvBinaries[stage]);
            std::string source = glslCompiler.compile();

            GLuint shaderId = glCreateShader(ShaderStageToGl(stage));
            componentIds.push_back(shaderId);
            const GLchar *codeArray[] = {source.c_str()};
            glShaderSource(shaderId, 1, codeArray, NULL);
            glCompileShader(shaderId);

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
    }

    GLenum GlShader::ShaderStageToGl(Shader::Stage stage)
    {
        CORE_ASSERT(stage != Stage::Invalid, "Invalid stage");

        switch (stage)
        {
        case Stage::Fragment:
            return GL_FRAGMENT_SHADER;
            break;

        case Stage::Vertex:
            return GL_VERTEX_SHADER;
            break;

        default:
            break;
        }

        CORE_ASSERT(false, "Not implemented stage conversion");
    }

    void GlShader::SpirvCompileOpenGL(std::vector<uint32_t> &target, const std::string &source, Shader::Stage stage, bool optimize)
    {
        CORE_DETAILED_PROFILE_FUNC();
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

        shaderc_optimization_level optimization = optimize ? shaderc_optimization_level_performance : shaderc_optimization_level_zero;
        options.SetOptimizationLevel(optimization);

        shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, ShaderStageToShadercKind(stage), dm_filePath.string().c_str(), options);
        CORE_ASSERT(result.GetCompilationStatus() == shaderc_compilation_status_success, "Shader {" + dm_name + "} failed to compile: " + result.GetErrorMessage());

        target = std::vector<uint32_t>(result.begin(), result.end());

        // TODO handle compiler errors
    }

}