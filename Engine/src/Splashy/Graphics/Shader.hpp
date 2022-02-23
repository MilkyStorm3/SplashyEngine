#pragma once
#include "Core/Core.hpp"
#include <filesystem>
#include <string>

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
        static Ref<Shader> Create(const std::filesystem::path &filePath, bool trackSource);

        /* Parses a shader file and splits to vertex and fragment source */
        virtual void LoadFromFile(const std::filesystem::path &filePath) = 0;

        /* Create from strings */
        virtual void FromSource(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc) = 0;

        /* Compiles and links shader */
        virtual void Init() = 0;

        /* Requires init call before */
        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual const std::string &GetName() const = 0;

        // todo material interface
    };

}