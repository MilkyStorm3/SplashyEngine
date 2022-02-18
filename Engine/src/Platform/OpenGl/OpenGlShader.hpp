#pragma once
#include <Graphics/Shader.hpp>
#include <unordered_map>
#include <GlTypes.h>
namespace ant::OpenGl
{

    class GlShader : public ant::Shader
    {
        // template<class T>
        // struct ShaderContainer
        // {
        //     T vertex;
        //     T fragment;
        // };

    public:
        GlShader();
        ~GlShader();

        virtual void LoadFromFile(const std::filesystem::path &filePath) override;
        virtual void Init() override;

        virtual void Bind() override;
        virtual void UnBind() override;

    private:
        static int CompileShader_classic(const std::string &source, uint32_t type);

        void GetVulcanBinaries(); //unused
        void GetOpenGlBinaries();

        void Parse(const std::filesystem::path& filePath);

    private:
        uint32_t m_glProgram;
        std::string m_name;
        std::filesystem::path m_filePath;

        std::unordered_map<GLenum,std::string> m_sources;
        std::unordered_map<GLenum,std::vector<uint32_t>> m_vulcanSPIRV;
        std::unordered_map<GLenum,std::vector<uint32_t>> m_openglSPIRV;
    };

} // namespace ant::OpenGl
