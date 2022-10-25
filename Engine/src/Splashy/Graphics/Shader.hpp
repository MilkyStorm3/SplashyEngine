#pragma once
#include "Core/Core.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp> // maybe avoid adding this include

namespace ant
{
    enum class RenderApi;

    class Shader
    {
    public:
        Shader() {}
        virtual ~Shader() {}

        /**
         * @param  filePath: path to a shader file in a custom hash separated format #vertexShader ... #fragmentShader ...
         * @retval Ref to initialized, linked, ready to use shader.
         */
        static Ref<Shader> Create(const std::filesystem::path &filePath, bool trackSource, bool useTextShaders);

        /* Parses a shader file and splits to vertex and fragment source */
        void LoadFromFile(const std::filesystem::path &filePath);

        /* Create from strings */
        void FromSource(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);

        /* Compiles and links shader */
        void Init();

        /* Requires init call before */
        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        inline const std::string &GetName() const { return dm_name; }

        // todo material interface

    // protected:
        friend class Lutils;
        enum Stage // make it protected later
        {
            Invalid = 0,
            Vertex,
            Fragment
        };
    protected:

        class Utils
        {
        public:
            static const char *GetCacheDirectory();
            static void CreateCacheDirectoryIfNeeded();
            static const char *GetDescriptiorFileExtension(Shader::Stage stage);
        };

        class Convert
        {
        public:
            static const char *StageEnumToStageString(Shader::Stage stage);
        };

    protected:
        virtual void OnInit() = 0;

    protected:
        std::string dm_name;
        bool dm_trackSourceFile = true;
        std::filesystem::path dm_filePath;
        std::unordered_map<Stage, std::string> dm_sources;
        std::unordered_map<Shader::Stage, nlohmann::json> dm_specifications;
        std::unordered_map<Shader::Stage, std::vector<uint32_t>> dm_SpirvBinaries;

    private:
        void Parse(const std::filesystem::path &filePath);
        void SpirvCompile(std::vector<uint32_t> &target, const std::string &source, Shader::Stage stage, bool optimize);
        void LoadOrCompileVBinary(const std::string &source, Stage stage, bool recompile = false);

        /* returns true if specification is loaded, false if there is no specs file */
        bool LoadSpecification(Shader::Stage stage);
        void CreateSpecification(const std::string &source, Shader::Stage stage);
    };

}