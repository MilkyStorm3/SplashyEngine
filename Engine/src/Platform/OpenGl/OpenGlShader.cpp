#include "OpenGlShader.hpp"

#include "debug/Instrumentation.hpp"

#include <Gl.h>
#include <filesystem>
#include <fstream>
#include <Core/Core.hpp>

#include <shaderc/shaderc.hpp>

namespace ant::OpenGl
{

    namespace Utils
    {
        static const char *GetCacheDirectory()
        {
            return "cache/shaders";
        }

        static const char *GetVulcanCacheFileExtension(GLenum stage)
        {
            if (stage == GL_VERTEX_SHADER)
                return ".vertvshader";
            if (stage == GL_FRAGMENT_SHADER)
                return ".pixelvshader";

            CORE_ASSERT(false, "Unsupported shader stage");
            return "FAIL";
        }

        static const char *GetOpenGlCacheFileExtension(GLenum stage)
        {
            if (stage == GL_VERTEX_SHADER)
                return ".vertglshader";
            if (stage == GL_FRAGMENT_SHADER)
                return ".pixelglshader";

            CORE_ASSERT(false, "Unsupported shader stage");
            return "FAIL";
        }

        static void CreateCacheDirectoryIfNeeded()
        {
            auto dir = GetCacheDirectory();
            if (!std::filesystem::exists(dir))
                std::filesystem::create_directories(dir);
        }

        static std::filesystem::path GetVulcanCachePath(const std::string &name, GLenum stage)
        {
            std::filesystem::path path = Utils::GetCacheDirectory();
            path.append(name + Utils::GetVulcanCacheFileExtension(stage));
            return path;
        }

        static std::filesystem::path GetOpenGlCachePath(const std::string &name, GLenum stage)
        {
            std::filesystem::path path = Utils::GetCacheDirectory();
            path.append(name + Utils::GetOpenGlCacheFileExtension(stage));
            return path;
        }

        static bool HasCachedBinary(const std::filesystem::path &path)
        {
            return std::filesystem::exists(path) && std::filesystem::file_size(path);
        }

        static shaderc_shader_kind GlEnumToShadercKind(GLenum type)
        {
            if (type == GL_VERTEX_SHADER)
                return shaderc_vertex_shader;
            if (type == GL_FRAGMENT_SHADER)
                return shaderc_fragment_shader;

            CORE_ASSERT(false, "Unsupported shader stage");
            return shaderc_miss_shader;
        }

        static const char *GlEnumToStageString(GLenum type)
        {
            if (type == GL_VERTEX_SHADER)
                return "vertex";
            if (type == GL_FRAGMENT_SHADER)
                return "fragment";

            CORE_ASSERT(false, "Unsupported shader stage");
            return "none";
        }

        static void LoadBinary(const std::filesystem::path &path, std::vector<uint32_t> *destination)
        {
            size_t dataSize = std::filesystem::file_size(path);

            std::ifstream inputCache(path, std::ios::binary);
            if (inputCache.is_open())
            {
                destination->resize(dataSize / sizeof(uint32_t));
                inputCache.read((char *)destination->data(), dataSize);
                inputCache.close();
            }
        }

        static void SaveBinary(const std::filesystem::path &path, std::vector<uint32_t> *data)
        {
            std::fstream outputCache(path, std::ios::binary | std::ios::out);

            if (outputCache.is_open())
            {
                outputCache.write((char *)data->data(), data->size() * sizeof(uint32_t));
                outputCache.flush();
                outputCache.close();
            }
        }

    } // namespace Utils

    GlShader::GlShader()
    {
        m_glProgram = GL_INVALID_INDEX;
        m_glProgram = glCreateProgram();

        CORE_ASSERT(m_glProgram != GL_INVALID_INDEX, "Failed do create glProgram (GlShader)");
    }

    GlShader::~GlShader()
    {
        glDeleteProgram(m_glProgram);
    }

    void GlShader::LoadFromFile(const std::filesystem::path &filePath)
    {
        CORE_PROFILE_FUNC();

        CORE_ASSERT(!filePath.empty(), "GlShader path not provided!");
        CORE_ASSERT(std::filesystem::exists(filePath), "Cannot find GlShader file! " + filePath.string());

        auto fname = filePath.filename().string();
        m_name = fname.substr(0, fname.find_last_of('.'));

        m_filePath = filePath;

        Parse(filePath);
    }

    void GlShader::FromSource(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        CORE_ASSERT(!name.empty(), "Name not provided!");
        CORE_ASSERT(!vertexSrc.empty(), "Vertex shader source not provided!");
        CORE_ASSERT(!fragmentSrc.empty(), "Fragment shader source not provided!");

        m_name = name;
        m_sources[GL_VERTEX_SHADER] = vertexSrc;
        m_sources[GL_FRAGMENT_SHADER] = fragmentSrc;
    }

    void GlShader::Init()
    {
        for (auto &[stage, source] : m_sources)
        {
            if (source.length() == 0)
            {
                std::stringstream msg;
                msg << "There has to be a " << Utils::GlEnumToStageString(stage) << " source!";
                CORE_ASSERT(false, msg.str().c_str());
            }
        }

        // GetVulcanBinaries();
        GetOpenGlBinaries();

        m_sources.clear();

        std::vector<GLenum> componentIds;
        for (auto &[stage, binary] : m_openglSPIRV)
        {
            GLuint shaderId = glCreateShader(stage);
            componentIds.push_back(shaderId);

            glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, binary.data(), binary.size() * sizeof(uint32_t));
            glSpecializeShader(shaderId, "main", 0, 0, 0);

            int res;
            glGetShaderiv(shaderId, GL_COMPILE_STATUS, &res);

            if (res == GL_FALSE)
            {
                std::stringstream ss;
                int len;
                glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &len);
                char *mes = (char *)alloca(len);
                glGetShaderInfoLog(shaderId, len, &len, mes);
                ss << Utils::GlEnumToStageString(stage) << " GlShader injection failed! " << mes;
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

    void GlShader::Bind()
    {
        glUseProgram(m_glProgram);
    }

    void GlShader::UnBind()
    {
        glUseProgram(0);
    }

    void GlShader::GetVulcanBinaries()
    {
        Utils::CreateCacheDirectoryIfNeeded();
        m_vulcanSPIRV.clear();

        for (auto &[stage, source] : m_sources)
        {
            const std::filesystem::path &path = Utils::GetVulcanCachePath(m_name, stage);

            if (Utils::HasCachedBinary(path))
            {
                Utils::LoadBinary(path, &m_vulcanSPIRV[stage]);
            }
            else
            {
                shaderc::Compiler compiler;
                shaderc::CompileOptions options;
                options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
                options.SetOptimizationLevel(shaderc_optimization_level_performance);
                shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, Utils::GlEnumToShadercKind(stage), m_filePath.c_str(), options);
                CORE_ASSERT(result.GetCompilationStatus() == shaderc_compilation_status_success, "Shader {" + m_name + "} failed to compile: " + result.GetErrorMessage());

                m_vulcanSPIRV[stage] = std::vector<uint32_t>(result.begin(), result.end());
                Utils::SaveBinary(path, &m_vulcanSPIRV[stage]);
            }
        }
    }

    void GlShader::GetOpenGlBinaries()
    {
        Utils::CreateCacheDirectoryIfNeeded();
        m_openglSPIRV.clear();

        for (auto &[stage, source] : m_sources)
        {
            const std::filesystem::path &path = Utils::GetOpenGlCachePath(m_name, stage);

            if (Utils::HasCachedBinary(path))
            {
                Utils::LoadBinary(path, &m_openglSPIRV[stage]);
            }
            else
            {
                shaderc::Compiler compiler;
                shaderc::CompileOptions options;
                options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
                options.SetOptimizationLevel(shaderc_optimization_level_performance);
                shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, Utils::GlEnumToShadercKind(stage), m_filePath.c_str(), options);
                CORE_ASSERT(result.GetCompilationStatus() == shaderc_compilation_status_success, "Shader {" + m_name + "} failed to compile: " + result.GetErrorMessage());

                m_openglSPIRV[stage] = std::vector<uint32_t>(result.begin(), result.end());
                Utils::SaveBinary(path, &m_openglSPIRV[stage]);
            }
        }
    }

    void GlShader::Parse(const std::filesystem::path &filePath)
    {

        std::ifstream file(filePath);
        GLenum type = GL_INVALID_ENUM;
        std::string line;

        while (std::getline(file, line))
        {
            if (!line.compare("#vertexShader"))
            {
                type = GL_VERTEX_SHADER;
                continue;
            }

            if (!line.compare("#fragmentShader"))
            {
                type = GL_FRAGMENT_SHADER;
                continue;
            }

            line.push_back('\n');
            CORE_ASSERT(type != GL_INVALID_ENUM, "Check shader definitions");
            m_sources[type].append(line);
            line.clear();
        }

        file.close();
    }

}