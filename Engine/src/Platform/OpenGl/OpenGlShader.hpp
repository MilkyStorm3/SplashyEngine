#pragma once
#include <Graphics/Shader.hpp>
#include <unordered_map>
#include <GlTypes.h>
#include <Render/RendererCommands.hpp>

namespace ant::OpenGl
{

    class GlShader : public ant::Shader
    {
    public:
        GlShader();
        ~GlShader();

        virtual void LoadFromFile(const std::filesystem::path &filePath) override;
        virtual void FromSource(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) override;
        virtual void Init() override;

        virtual void Bind() override;
        virtual void UnBind() override;

        virtual const std::string &GetName() override { return m_name; }

    private:
        void Parse(const std::filesystem::path &filePath);
        void ComposeGlProgram();

        void GetVulcanBinaries(); // unused
        void GetOpenGlBinaries();
        void CompileSpirv( std::vector<uint32_t>& target, const std::string& source, GLenum stage, RenderApi targetEnv, bool optimize); 
	    void CreateDesctiptorIfNeeded();

    private:
        uint32_t m_glProgram;
        std::string m_name;
        std::filesystem::path m_filePath;

        std::unordered_map<GLenum, std::string> m_sources;
        std::unordered_map<GLenum, std::vector<uint32_t>> m_vulcanSPIRV;
        std::unordered_map<GLenum, std::vector<uint32_t>> m_openglSPIRV;
    };

} // namespace ant::OpenGl
