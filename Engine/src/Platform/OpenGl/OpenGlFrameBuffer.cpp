#include "Pch.h"
#include "OpenGlFrameBuffer.hpp"
#include <Gl.h>
#include <Utilities/InstrumentationMacros.hpp>
#include "Core/Logger.hpp"

namespace ant::OpenGl
{

    namespace Utils
    {

        static bool IsDepthBuffer(FramebufferTextureFormat format)
        {
            return format == FramebufferTextureFormat::DEPTH24STENCIL8;
        }

        static GLenum FBTextureFormatToGL(FramebufferTextureFormat format)
        {
            switch (format)
            {
            case FramebufferTextureFormat::RGBA8:
                return GL_RGBA8;
            case FramebufferTextureFormat::RED_INTEGER:
                return GL_RED_INTEGER;
            }

            CORE_ASSERT(false, "Incorrect color attachment format");
            return 0;
        }

    }

    GlFrameBuffer::GlFrameBuffer(const FramebufferSpecification &specs)
        : m_specs(specs)
    {
        CORE_DETAILED_PROFILE_FUNC();
        glCreateFramebuffers(1, &m_frameBufferGlId);

        bool hasDepth = false;

        for (auto &spec : specs.AttachmentSpecification.Attachments)
        {

            GlAttachmentInfo newAttachment;
            newAttachment.Format = spec;

            glCreateTextures(GL_TEXTURE_2D, 1, &newAttachment.GlId);

            if (Utils::IsDepthBuffer(spec))
            {
                CORE_ASSERT(hasDepth == false, "Framebuffer can have only one depth attachment!");
                m_depthAttachment = newAttachment;
                hasDepth = true;
                continue;
            }

            m_colorAttachments.push_back(newAttachment);
        }

        Invalidate();
    }

    GlFrameBuffer::~GlFrameBuffer()
    {
        CORE_DETAILED_PROFILE_FUNC();
        glDeleteFramebuffers(1, &m_frameBufferGlId);

        for (auto &att : m_colorAttachments)
            glDeleteTextures(1, &att.GlId);

        glDeleteTextures(1, &m_depthAttachment.GlId);
    }

    void GlFrameBuffer::Bind()
    {
        CORE_DETAILED_PROFILE_FUNC();
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferGlId);
        glViewport(0, 0, m_specs.width, m_specs.height);
    }

    void GlFrameBuffer::UnBind()
    {
        CORE_DETAILED_PROFILE_FUNC();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GlFrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        CORE_DETAILED_PROFILE_FUNC();
        m_specs.width = width;
        m_specs.height = height;

        Invalidate();
    }

    uint32_t GlFrameBuffer::GetColorAttachmentRendererId(uint32_t index)
    {
        CORE_ASSERT(index < m_colorAttachments.size(), "Invalid color attachment index");
        return m_colorAttachments.at(index).GlId;
    }

    void GlFrameBuffer::ClearAttachment(uint32_t index, int value)
    {
        CORE_ASSERT(index < m_colorAttachments.size(), "Invalid color attachment index");

        auto &att = m_colorAttachments.at(index);
        glClearTexImage(att.GlId, 0, Utils::FBTextureFormatToGL(att.Format), GL_INT, &value);
    }

    int GlFrameBuffer::ReadPixel(uint32_t index, int x, int y)
    {
        CORE_ASSERT(index < m_colorAttachments.size(), "Invalid color attachment index");

        glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    void GlFrameBuffer::Invalidate()
    {
        CORE_DETAILED_PROFILE_FUNC();

        if (m_depthAttachment.GlId)
            glDeleteTextures(1, &m_depthAttachment.GlId);

        CORE_ASSERT(m_colorAttachments.size() <= 16, "Framebuffer may have up to 16 color attachments");

        // color
        for (size_t i = 0; i < m_colorAttachments.size(); i++)
        {
            auto &attachment = m_colorAttachments[i];
            if (attachment.GlId)
                glDeleteTextures(1, &attachment.GlId);

            glCreateTextures(GL_TEXTURE_2D, 1, &attachment.GlId);
            glTextureStorage2D(attachment.GlId, 1, GL_RGBA8, m_specs.width, m_specs.height);
            glNamedFramebufferTexture(m_frameBufferGlId, GL_COLOR_ATTACHMENT0 + i, attachment.GlId, 0);
        }

        // depth
        glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment.GlId);
        glTextureStorage2D(m_depthAttachment.GlId, 1, GL_DEPTH24_STENCIL8, m_specs.width, m_specs.height);
        glNamedFramebufferTexture(m_frameBufferGlId, GL_DEPTH_STENCIL_ATTACHMENT, m_depthAttachment.GlId, 0);

        CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer is not complete!");
    }
}