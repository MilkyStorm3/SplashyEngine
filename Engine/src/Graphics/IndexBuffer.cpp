#include "IndexBuffer.hpp"
#include <Core/Core.hpp>

#include <Gl.h>

namespace ant
{

    IndexBuffer::IndexBuffer()
    {
        m_glId = GL_INVALID_INDEX;
        glCreateBuffers(1, &m_glId);
        CORE_ASSERT(m_glId != GL_INVALID_INDEX, "Failed to create glIndexBuffer!");
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_glId);
    }

    void IndexBuffer::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glId);
    }

    void IndexBuffer::UnBind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void IndexBuffer::UploadData(uint32_t *data, size_t size)
    {
        CORE_ASSERT(size, "Size cannot be 0");
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        m_count = size;
    }

}