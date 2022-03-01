#pragma once
#include "Core/Core.hpp"
#include <vector>

namespace ant
{

    enum class FramebufferTextureFormat
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

    struct FramebufferAtachmentSpecification
    {
        FramebufferAtachmentSpecification(std::initializer_list<FramebufferTextureFormat> formats) : Attachments(formats) {}

        std::vector<FramebufferTextureFormat> Attachments;
    };

    struct FramebufferSpecification
    {
        uint32_t width, height;
        FramebufferAtachmentSpecification AttachmentSpecification;
    };

    class FrameBuffer
    {
    public:
        FrameBuffer() {}
        FrameBuffer(const FrameBuffer &) = delete;
        virtual ~FrameBuffer() {}

        static Ref<FrameBuffer> Create(const FramebufferSpecification &specs);

        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual uint32_t GetSizeX() = 0;
        virtual uint32_t GetSizeY() = 0;

        virtual uint32_t GetColorAttachmentRendererId(uint32_t index) = 0;
        virtual void ClearAttachment(uint32_t index, int value) = 0;
        virtual int ReadPixel(uint32_t index, int x, int y) = 0;
    };

}