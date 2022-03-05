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
        GlShader(bool trackSource);
        ~GlShader();

        virtual void LoadFromFile(const std::filesystem::path &filePath) override;
        virtual void FromSource(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc) override;
        virtual void Init() override;

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual const std::string &GetName() const override { return m_name; }

    private:
        void Parse(const std::filesystem::path &filePath);
        void ComposeGlProgram();

        void GetVulcanBinary(const std::string &source, GLenum stage, bool forceRecompilation);
        void GetOpenGlBinary(GLenum stage, bool forceRecompilation);
        void CompileSpirv(std::vector<uint32_t> &target, const std::string &source, GLenum stage, RenderApi targetEnv, bool optimize);

        /* returns true if specification is loaded, false if there is no specs file */
        bool LoadSpecification(GLenum stage);
        void CreateSpecification(const std::string &source, GLenum stage);

    private:
        uint32_t m_glProgram;
        std::string m_name;
        std::filesystem::path m_filePath;
        bool m_trackingSource = true;

        std::unordered_map<GLenum, std::string> m_sources;
        std::unordered_map<GLenum, nlohmann::json> m_specifications;
        std::unordered_map<GLenum, std::vector<uint32_t>> m_vulcanSPIRV;
        std::unordered_map<GLenum, std::vector<uint32_t>> m_openglSPIRV;
    };

} // namespace ant::OpenGl
