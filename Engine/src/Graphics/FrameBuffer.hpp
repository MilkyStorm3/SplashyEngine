#pragma once
#include "Core/Core.hpp"

namespace ant
{

    class FrameBuffer
    {
    public:
        static Ref<FrameBuffer> Create(uint32_t width, uint32_t height);

        FrameBuffer(uint32_t width, uint32_t height);
        ~FrameBuffer();

        void Bind();

        static void BindDefault();

        void Resize(uint32_t width, uint32_t height);
        void Invalidate();

        uint32_t GetGlId() const { return m_colorBufferId; }

    private:
        uint32_t m_width, m_height;
        uint32_t m_colorBufferId = 0, m_depthBufferId = 0, m_frameBufferGlId = 0;
    };

}