#pragma once
#include <Graphics/IndexBuffer.hpp>

namespace ant::OpenGl
{

    class GlIndexBuffer : public ant::IndexBuffer
    {
    public:
        GlIndexBuffer();
        ~GlIndexBuffer();

        virtual void Bind() override;
        virtual void UnBind() override;
        virtual void UploadData(uint32_t *data, size_t size) override;

        virtual uint32_t GetCount() const override { return m_count; }

    private:
        uint32_t m_count;
        uint32_t m_glId;
    };

} // namespace ant::OpenGl
