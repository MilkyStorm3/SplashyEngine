#pragma once
#include <filesystem>
#include <string>

namespace ant
{

    class Shader
    {
    private:
        struct ShaderSource
        {
            std::string vertex;
            std::string fragment;
        };

    public:
        Shader();
        ~Shader();

        /* Parses a shader file and splits to vertex and fragment source */
        void LoadFromFile(const std::string &filePath);

        /* Compiles and links shader */
        void Init();

        /* Compiles and links shader from given source strings*/
        void Init(const std::string &vertexsrc, const std::string &fragmentsrc);

        /* Requires init call before */
        void Bind();
        void UnBind();

        const ShaderSource &GetSource() const { return m_source; }

    private:
        static int CompileShader(const std::string &source, uint32_t type);

        ShaderSource m_source;
        uint32_t m_glProgram;
    };

}