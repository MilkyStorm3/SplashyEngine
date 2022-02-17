#pragma once
#include <Graphics/Shader.hpp>

namespace ant::OpenGl
{

    class GlShader : public ant::Shader
    {
        struct ShaderSource
        {
            std::string vertex;
            std::string fragment;
        };

    public:
        GlShader();
        ~GlShader();

        /* Parses a shader file and splits to vertex and fragment source */
        virtual void LoadFromFile(const std::string &filePath) override;

        /* Compiles and links shader */
        virtual void Init() override;

        /* Requires init call before */
        virtual void Bind() override;
        virtual void UnBind() override;

    private:
        static int CompileShader(const std::string &source, uint32_t type);

        ShaderSource m_source;
        uint32_t m_glProgram;
    };

} // namespace ant::OpenGl
