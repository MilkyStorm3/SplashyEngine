#include "OpenGlVertexBuffer.hpp"
#include <Utilities/InstrumentationMacros.hpp>
#include <Core/Core.hpp>
#include <GL/glew.h>

namespace ant::OpenGl
{
    GlVertexBuffer::GlVertexBuffer()
    {
        m_layout = MakeRef<GlVertexLayout>();

        CORE_DETAILED_PROFILE_FUNC();

        m_glId = GL_INVALID_INDEX;

        glCreateBuffers(1, &m_glId);

        CORE_ASSERT(m_glId != GL_INVALID_INDEX, "Failed to create glVertexBuffer");
    }

    GlVertexBuffer::~GlVertexBuffer()
    {
        CORE_DETAILED_PROFILE_FUNC();

        glDeleteBuffers(1, &m_glId);
    }

    void GlVertexBuffer::Bind() const
    {
        CORE_ASSERT(false, "NO IMPLEMENTATION!");
    }

    void GlVertexBuffer::UnBind() const
    {
        CORE_ASSERT(false, "NO IMPLEMENTATION!");
    }

    void GlVertexBuffer::UploadData(float *data, size_t size)
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        m_sizeBytes = size;
        glNamedBufferData(m_glId, size, data, GL_STATIC_DRAW);
    }

}