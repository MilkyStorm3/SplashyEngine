#include "Pch.h"
#include "OpenGlFrameBuffer.hpp"
#include <Gl.h>

namespace ant::OpenGl
{

    GlFrameBuffer::GlFrameBuffer(uint32_t width, uint32_t height)
        : m_width(width), m_height(height)
    {
        CORE_PROFILE_FUNC();
        glGenFramebuffers(1, &m_frameBufferGlId);
        Invalidate();
    }

    GlFrameBuffer::~GlFrameBuffer()
    {
        CORE_PROFILE_FUNC();
        glDeleteFramebuffers(1, &m_frameBufferGlId);
        glDeleteTextures(1, &m_colorBufferId);
        glDeleteTextures(1, &m_depthBufferId);
    }

    void GlFrameBuffer::Bind()
    {
        CORE_PROFILE_FUNC();
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferGlId);
        glViewport(0, 0, m_width, m_height);
    }

    void GlFrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GlFrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        CORE_PROFILE_FUNC();
        m_width = width;
        m_height = height;

        Invalidate();
    }

    void GlFrameBuffer::Invalidate()
    {
        CORE_PROFILE_FUNC();

        if (m_colorBufferId)
            glDeleteTextures(1, &m_colorBufferId);

        if (m_depthBufferId)
            glDeleteTextures(1, &m_depthBufferId);

        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferGlId);

        // color
        glCreateTextures(GL_TEXTURE_2D, 1, &m_colorBufferId);
        glTextureStorage2D(m_colorBufferId, 1, GL_RGBA8, m_width, m_height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBufferId, 0);

        // depth
        glCreateTextures(GL_TEXTURE_2D, 1, &m_depthBufferId);
        glTextureStorage2D(m_depthBufferId, 1, GL_DEPTH24_STENCIL8, m_width, m_height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthBufferId, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer is not complete!");
    }
}