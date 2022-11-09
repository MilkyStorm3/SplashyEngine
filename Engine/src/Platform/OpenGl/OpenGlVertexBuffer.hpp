#pragma once
#include <Graphics/VertexBuffer.hpp>
#include "OpenGlVertexLayout.hpp"

namespace ant::OpenGl
{
    class GlVertexBuffer : public ant::VertexBuffer
    {
    public:
        GlVertexBuffer();
        ~GlVertexBuffer();
        virtual void Bind() const override;
        virtual void UnBind() const override;
        virtual void UploadData(float *data, size_t size) override;
        virtual Ref<VertexLayout> GetLayout() override { return m_layout; }
        virtual void SetLayout(Ref<VertexLayout> layout) override { m_layout = layout; }

        virtual uint32_t GetRendererId() const override { return m_glId; }

        virtual size_t GetSize() const override { return m_sizeBytes; }

    private:
        uint32_t m_glId;
        size_t m_sizeBytes;
        Ref<VertexLayout> m_layout;
    };
} // namespace ant::OpenGl
