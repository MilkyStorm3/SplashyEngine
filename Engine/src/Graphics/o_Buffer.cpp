#include "Graphics/o_Buffer.hpp"
#ifdef OLD_BUFFER
#include "Pch.h"

#include <Gl.h>

#include <GLFW/glfw3.h>

namespace ant
{

    Buffer::Buffer(BufferType bufferType)
    {
        switch (bufferType)
        {
        case BufferType::VertexBuffer:
            m_glBufferType = GL_ARRAY_BUFFER;
            break;

        case BufferType::IndexBuffer:
            m_glBufferType = GL_ELEMENT_ARRAY_BUFFER;
            break;

        default:
            break;
        }
    }

    void Buffer::Bind()
    {
        CORE_PROFILE_FUNC();
        BindVertexArrayObj();
        glBindBuffer(m_glBufferType, m_glId);
    }

    Buffer::~Buffer()
    {
        glDeleteBuffers(1, &m_glId);
    }

    void Buffer::BufferData(void *array, size_t size, bool dynamicDraw)
    {
        CORE_PROFILE_FUNC();
        Create();
        Bind();
        glBufferData(m_glBufferType, 4 * size, array, (dynamicDraw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
        // 4 sizeof uint32_t and float
    }

    VertexBuffer::GlVertexArray::~GlVertexArray()
    {
        glDeleteVertexArrays(1, &m_glId);
    }

    void VertexBuffer::GlVertexArray::Bind()
    {
        glBindVertexArray(m_glId);
    }

    void VertexBuffer::GlVertexArray::Create()
    {
        glGenVertexArrays(1, &m_glId);
    }

    VertexBuffer::VertexBuffer()
        : Buffer(BufferType::VertexBuffer)
    {
    }

    void VertexBuffer::Create()
    {
        glCreateBuffers(1, &m_glId);
        m_glVertexArray.Create();
    }

    void VertexBuffer::BindVertexArrayObj()
    {
        m_glVertexArray.Bind();
    }

    void IndexBuffer::Create()
    {
        glCreateBuffers(1, &m_glId);
    }

    void VertexArrayPrimitive::Bind()
    {
        CORE_PROFILE_FUNC();
        m_vertexBuffer.Bind();
        m_indexBuffer.Bind();
    }

    void VertexBuffer::UpdateLayout()
    {
        CORE_PROFILE_FUNC();
        m_glVertexArray.Bind();
        m_layout.Enable();
    }

} // namespace ant

#endif