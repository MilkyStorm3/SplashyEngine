#include "OpenGlUniformBuffer.hpp"
#include <Gl.h>

namespace ant::OpenGl
{
    GlUniformBuffer::GlUniformBuffer(uint32_t size, uint32_t binding)
    {
        glCreateBuffers(1, &m_glId);
        glNamedBufferData(m_glId, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_glId);
    }

    GlUniformBuffer::~GlUniformBuffer()
    {
        glDeleteBuffers(1, &m_glId);
    }

    void GlUniformBuffer::SetData(const void *data, uint32_t size, uint32_t offset)
    {
        glNamedBufferSubData(m_glId, offset, size, data);
    }
}