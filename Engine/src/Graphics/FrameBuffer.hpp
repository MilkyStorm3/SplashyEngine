#pragma once
#include "Core/Core.hpp"
#include <vector>

namespace ant
{

    enum class FramebufferAtachmentType
    {
        None = 0,

        // Color
        RGBA8,
        RED_INTEGER,

        // Depth/stencil
        DEPTH24STENCIL8,

        // Defaults
        Depth = DEPTH24STENCIL8
    };

    class FramebufferAtachments
    {
    public:
        FramebufferAtachments() = default;
        FramebufferAtachments(std::initializer_list<FramebufferAtachmentType> attachments) : Attachments(attachments) {}

        std::vector<FramebufferAtachmentType> Attachments;
    };

    class FrameBuffer
    {
    public:
        static Ref<FrameBuffer> Create(uint32_t width, uint32_t height);

        FrameBuffer(uint32_t width, uint32_t height);
        ~FrameBuffer();

        void Bind();
        inline void UnBind() const { FrameBuffer::BindDefault(); }

        static void BindDefault();

        void Resize(uint32_t width, uint32_t height);

        // uint32_t GetColorBufferId() const { return m_colorBufferId; }

    private:
        void Invalidate();

    private:
        uint32_t m_width, m_height;
        uint32_t m_colorBufferId = 0, m_depthBufferId = 0, m_frameBufferGlId = 0;
    };

}