#include "OpenGlIndexBuffer.hpp"
#include <Core/Core.hpp>
#include <GL/glew.h> 

namespace ant::OpenGl
{
    GlIndexBuffer::GlIndexBuffer()
    {
        CORE_DETAILED_PROFILE_FUNC();
        m_glId = GL_INVALID_INDEX;
        glCreateBuffers(1, &m_glId);
        CORE_ASSERT(m_glId != GL_INVALID_INDEX, "Failed to create glIndexBuffer!");
    }

    GlIndexBuffer::~GlIndexBuffer()
    {
        CORE_DETAILED_PROFILE_FUNC();
        glDeleteBuffers(1, &m_glId);
    }

    void GlIndexBuffer::Bind() const
    {
        CORE_ASSERT(false, "NO IMPLEMENTATION!");
    }

    void GlIndexBuffer::UnBind() const
    {
        CORE_ASSERT(false, "NO IMPLEMENTATION!");
    }

    void GlIndexBuffer::UploadData(uint32_t *data, size_t size)
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        CORE_ASSERT(size, "Size cannot be 0");
        glNamedBufferData(m_glId, size, data, GL_STATIC_DRAW);
        m_count = size / sizeof(uint32_t);
    }
}