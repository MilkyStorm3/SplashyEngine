#include "OpenGlIndexBuffer.hpp"
#include <Core/Core.hpp>
#include <Gl.h>

namespace ant::OpenGl
{
    GlIndexBuffer::GlIndexBuffer()
    {
        m_glId = GL_INVALID_INDEX;
        glCreateBuffers(1, &m_glId);
        CORE_ASSERT(m_glId != GL_INVALID_INDEX, "Failed to create glIndexBuffer!");
    }

    GlIndexBuffer::~GlIndexBuffer()
    {
        glDeleteBuffers(1, &m_glId);
    }

    void GlIndexBuffer::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glId);
    }

    void GlIndexBuffer::UnBind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void GlIndexBuffer::UploadData(uint32_t *data, size_t size)
    {
        Bind();
        CORE_ASSERT(size, "Size cannot be 0");
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        m_count = size / sizeof(uint32_t);
        UnBind();
    }
}