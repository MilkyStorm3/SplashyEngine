#pragma once
#include <Graphics/Texture.hpp>
#include "GlTypes.h"

namespace ant::OpenGl
{
    class GlTexture2D : public ant::Texture2D
    {
    public:
        GlTexture2D(const std::filesystem::path &path);
        GlTexture2D(uint32_t width, uint32_t height);
        ~GlTexture2D();

        virtual uint32_t GetSizeX() const override;
        virtual uint32_t GetSizeY() const override;
        virtual uint32_t GetRendererId() const override;
        virtual void Bind(uint32_t slot = 0) const override;
        virtual void SetData(void *data, uint32_t size) override;
        virtual bool IsLoaded() const override;

        virtual void SetBorderColor(const glm::vec4 &color) override;
        virtual void SetLevelOfDetail(float min = -1000.f, float max = 1000.f) override;
        virtual void SetParameter(TextureParam param, TextureParamValue value) override;

    private:
        uint32_t m_glID;
        uint16_t m_width;
        uint16_t m_height;
        GLenum m_inputFormat;
        bool m_isLoaded = false;
    };

} // namespace ant::OpenGl
