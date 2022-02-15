#pragma once
#include <Graphics/FrameBuffer.hpp>

namespace ant::OpenGl
{

  class GlFrameBuffer : public ant::FrameBuffer
  {
  public:
    GlFrameBuffer(uint32_t width, uint32_t height);
    ~GlFrameBuffer();

    virtual void Bind() override;
    virtual void UnBind() override;

    virtual void Resize(uint32_t width, uint32_t height) override;
    virtual inline uint32_t GetSizeX() override { return m_width; }
    virtual inline uint32_t GetSizeY() override { return m_height; }
    // todo implement attachments

    uint32_t GetColorBufferId() const { return m_colorBufferId; } //! TEMP

  private:
    void Invalidate();

  private:
    uint32_t m_width, m_height;
    uint32_t m_colorBufferId = 0, m_depthBufferId = 0, m_frameBufferGlId = 0;
  };

}
