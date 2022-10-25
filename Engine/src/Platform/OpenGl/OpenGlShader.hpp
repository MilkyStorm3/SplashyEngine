#pragma once
#include <Graphics/Shader.hpp>
#include <unordered_map>
#include "GlTypes.h"
#include <nlohmann/json.hpp>
#include <Render/RendererCommands.hpp>

namespace ant::OpenGl
{

    class GlShader : public ant::Shader
    {
    public:
        GlShader(bool trackSource, bool useTextShaders);
        ~GlShader();

        virtual void OnInit() override;

        virtual void Bind() const override;
        virtual void UnBind() const override;

    private:
        void ComposeGlProgram();
        void ComposeGlProgramFromSource();

        void GetOpenGlBinary(Shader::Stage stage, bool forceRecompilation);

        //--------------------------------------------------------------------------------

        static GLenum ShaderStageToGl(Shader::Stage stage);
        static const char *GetOpenGlCacheFileExtension(GLenum stage);
        static std::filesystem::path GetOpenGlCachePath(const std::string &name, GLenum stage);

    private:
        void SpirvCompileOpenGL(std::vector<uint32_t> &target, const std::string &source, Shader::Stage stage, bool optimize);
        uint32_t m_glProgram;
        bool m_useTextShaders = false;

        std::unordered_map<Shader::Stage, std::vector<uint32_t>> m_openglBinaries;
    };

} // namespace ant::OpenGl
