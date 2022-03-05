#pragma once
#include <Graphics/IndexBuffer.hpp>

namespace ant::OpenGl
{

    class GlIndexBuffer : public ant::IndexBuffer
    {
    public:
        GlIndexBuffer();
        ~GlIndexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;
        virtual void UploadData(uint32_t *data, size_t size) override;

        virtual uint32_t GetCount() const override { return m_count; }

        virtual uint32_t GetRendererId() const override { return m_glId; }

    private:
        uint32_t m_count;
        uint32_t m_glId;
    };

} // namespace ant::OpenGl
