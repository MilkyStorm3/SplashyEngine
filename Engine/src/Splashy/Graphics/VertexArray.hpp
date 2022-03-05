#pragma once
#include <vector>
#include "Core/Memory.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"

namespace ant
{

    class VertexArray
    {
    public:
        VertexArray() {}
        virtual ~VertexArray() {}

        static Ref<VertexArray> Create();

        virtual void Bind() const = 0;

        virtual void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) = 0;

        virtual void SetIndexBuffer(Ref<IndexBuffer> indexBuffer) = 0;

        virtual const std::vector<Ref<VertexBuffer>> &GetVertexBuffers() const = 0;
        virtual const Ref<IndexBuffer> &GetIndexBuffer() const = 0;
    };

} // namespace ant
