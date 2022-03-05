#pragma once
#include <Core/Core.hpp>
#include <Graphics/VertexArray.hpp>

namespace ant::OpenGl
{
    
    class GlVertexArray : public ant::VertexArray
    {
    public:
        GlVertexArray();
        ~GlVertexArray() override;

        virtual void Bind() const override;
        virtual void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) override;
        virtual void SetIndexBuffer(Ref<IndexBuffer> indexBuffer) override;
        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override;

    private:
        uint32_t m_glId;
        std::vector<Ref<VertexBuffer>> m_vertexBuffers;
        Ref<IndexBuffer> m_indexBuffer;
    };


} // namespace ant
