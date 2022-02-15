#pragma once
#include "Core/Core.hpp"
namespace ant
{

    class FrameBuffer
    {
    public:
        FrameBuffer() {}
        FrameBuffer(const FrameBuffer &) = delete;
        virtual ~FrameBuffer() {}

        static Ref<FrameBuffer> Create(uint32_t width, uint32_t height);

        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual uint32_t GetSizeX() = 0;
        virtual uint32_t GetSizeY() = 0;
        // todo implement attachments
    };

}