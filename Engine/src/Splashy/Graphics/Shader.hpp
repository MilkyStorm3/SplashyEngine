#pragma once
#include <filesystem>
#include <string>
#include "Core/Core.hpp"

namespace ant
{

    class Shader
    {
    public:
        Shader() {}
        virtual ~Shader() {}

        /**
         * @param  filePath: path to a shader file in a custom hash separated format #vertexShader ... #fragmentShader ...
         * @retval Ref to initialized, linked, ready to use shader.
         */
        static Ref<Shader> Create(const std::filesystem::path &filePath);

        /* Parses a shader file and splits to vertex and fragment source */
        virtual void LoadFromFile(const std::filesystem::path &filePath) = 0;

        /* Compiles and links shader */
        virtual void Init() = 0;

        /* Requires init call before */
        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        //todo uniforms, uniform buffers
    };

}