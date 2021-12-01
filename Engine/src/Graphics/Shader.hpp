#pragma once
#include <filesystem>
#include <string>
#include "Core/Core.hpp"

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

        /**
         * @param  filePath: path to a shader file in a custom hash separated format #vertexShader ... #fragmentShader ...
         * @retval Ref to initialized, linked, ready to use shader.
         */
        static Ref<Shader> Create(const std::string &filePath);

        /**
         * @param  vertexsrc: vertex shader raw source string
         * @param  fragmentsrc: fragment shader raw source string
         * @retval Ref to initialized, linked, ready to use shader.
         */
        static Ref<Shader> Create(const std::string &vertexsrc, const std::string &fragmentsrc);

        /* Parses a shader file and splits to vertex and fragment source */
        void LoadFromFile(const std::string &filePath);

        /* Compiles and links shader */
        void Init();

        /* Compiles and links shader from given source strings*/
        void Init(const std::string &vertexsrc, const std::string &fragmentsrc);

        /* Requires init call before */
        void Bind();
        void UnBind();

        /**
         * @retval Struct with vertex and fragment shader source strings
         */
        const ShaderSource &GetSource() const { return m_source; }

    private:
        static int CompileShader(const std::string &source, uint32_t type);

        ShaderSource m_source;
        uint32_t m_glProgram;
    };

}