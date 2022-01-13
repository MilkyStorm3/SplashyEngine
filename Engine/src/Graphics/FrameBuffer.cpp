#include "Pch.h"
#include "Render/Renderer.hpp"
#include "Graphics/FrameBuffer.hpp"
#include <Gl.h>

#include "Core/Logger.hpp"

namespace ant
{

    namespace utils
    {

        static bool IsColorAttachment(FramebufferAtachmentType type)
        {
            if (type == FramebufferAtachmentType::DEPTH24STENCIL8)  return true;
            if (type == FramebufferAtachmentType::RED_INTEGER)      return true;

            return false;
        }

    }

    Ref<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height)
    {
        return MakeRef<FrameBuffer>(width, height);
    }

    FrameBuffer::FrameBuffer(uint32_t width, uint32_t height)
        : m_width(width), m_height(height)
    {
        glGenFramebuffers(1, &m_frameBufferGlId);
        Invalidate();
    }

    FrameBuffer::~FrameBuffer()
    {
        glDeleteFramebuffers(1, &m_frameBufferGlId);
        glDeleteTextures(1, &m_colorBufferId);
        glDeleteTextures(1, &m_depthBufferId);
    }

    void FrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferGlId);
        glViewport(0, 0, m_width, m_height);
    }

    void FrameBuffer::BindDefault()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        m_width = width;
        m_height = height;

        Invalidate();
    }

    void FrameBuffer::Invalidate()
    {

        if (m_colorBufferId)
            glDeleteTextures(1, &m_colorBufferId);

        if (m_depthBufferId)
            glDeleteTextures(1, &m_depthBufferId);

        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferGlId);

        //color
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