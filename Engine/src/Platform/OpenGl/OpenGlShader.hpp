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

        GlShader(const GlShader &) = delete;
        GlShader(GlShader &&) = delete;
        ~GlShader();

        virtual void OnInit() override;
        virtual void Bind() const override;
        virtual void UnBind() const override;

    private:
        friend class OpenGlShaderUtils;

    private:
        void GetOpenGlBinary(Shader::Stage stage, bool forceRecompilation);
        void SpirvCompileOpenGL(std::vector<uint32_t> &target, const std::string &source, Shader::Stage stage, bool optimize);

    private:
        uint32_t m_glProgram;
        bool m_useTextShaders = false;
        Scope<StageMap<std::vector<uint32_t>>> m_openglBinaries;
    };

} // namespace ant::OpenGl
