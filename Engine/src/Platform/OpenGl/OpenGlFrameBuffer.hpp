#pragma once
#include <Graphics/FrameBuffer.hpp>

namespace ant::OpenGl
{

  struct GlAttachmentInfo
  {
    GlAttachmentInfo() {}

    uint32_t GlId;
    FramebufferTextureFormat Format;
  };

  class GlFrameBuffer : public ant::FrameBuffer
  {
  public:
    GlFrameBuffer(const FramebufferSpecification &specs);
    ~GlFrameBuffer();

    virtual void Bind() override;
    virtual void UnBind() override;

    virtual void Resize(uint32_t width, uint32_t height) override;
    virtual inline uint32_t GetSizeX() override { return m_specs.width; }
    virtual inline uint32_t GetSizeY() override { return m_specs.height; }

    virtual uint32_t GetColorAttachmentRendererId(uint32_t index) override;
    virtual void ClearAttachment(uint32_t index, int value) override;
    virtual void SetBlendingMode(BlendingMode source, BlendingMode current) override;
    virtual int ReadPixel(uint32_t index, int x, int y) override;

  private:
    void Invalidate();

  private:
    uint32_t m_frameBufferGlId = 0;
    FramebufferSpecification m_specs;

    GlAttachmentInfo m_depthAttachment;
    std::vector<GlAttachmentInfo> m_colorAttachments;
  };

}
