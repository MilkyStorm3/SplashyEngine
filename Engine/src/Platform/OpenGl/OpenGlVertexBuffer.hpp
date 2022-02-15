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
        virtual void Bind() override;
        virtual void UnBind() override;
        virtual void UploadData(float *data, size_t size) override;
        virtual Ref<VertexLayout> GetLayout() override { return m_layout; }
        virtual void SetLayout(Ref<VertexLayout> layout) override { m_layout = layout; }

    private:
        uint32_t m_glId;
        uint32_t m_vertexArrayGlId;
        Ref<VertexLayout> m_layout;
    };
} // namespace ant::OpenGl
