#pragma once
#include "Core/Core.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp> // maybe avoid adding this include

namespace ant
{
    class Shader
    {
    public:
        Shader(bool sourceTracking);

        virtual ~Shader() {}

        Shader(const Shader &) = delete;
        Shader(Shader &&) = delete;

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

    protected:
        friend class Lutils;

        enum Stage
        {
            Invalid = 0,
            Vertex,
            Fragment
        };

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

        template <class T>
        using StageMap = std::unordered_map<Shader::Stage, T>;

    protected:
        virtual void OnInit() = 0;

    protected:
        std::string dm_name;
        std::filesystem::path dm_filePath;

        StageMap<nlohmann::json> dm_specifications;
        StageMap<bool> dm_recompilePlatformShaders;

        Scope<StageMap<std::vector<uint32_t>>> dm_SpirvBinaries;

    private:
        void Parse(const std::filesystem::path &filePath);
        void SpirvCompile(std::vector<uint32_t> &target, const std::string &source, Shader::Stage stage, bool optimize);
        void LoadOrCompileVBinary(const std::string &source, Stage stage, bool recompile = false);

        /* returns true if specification is loaded, false if there is no specs file */
        bool LoadSpecification(Shader::Stage stage);
        void CreateSpecification(const std::string &source, Shader::Stage stage);

    private:
        Scope<StageMap<std::string>> m_sources;
        bool m_trackSourceFile = true;
    };

}