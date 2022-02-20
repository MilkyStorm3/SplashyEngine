#pragma once
#include <Graphics/UniformBuffer.hpp>

namespace ant::OpenGl
{

    class GlUniformBuffer : public ant::UniformBuffer
    {
    public:
        GlUniformBuffer(uint32_t size, uint32_t binding);
        ~GlUniformBuffer();

        virtual void SetData(const void *data, uint32_t size, uint32_t offset = 0) override;

    private:
        uint32_t m_glId;
    };

} // namespace ant::OpenGl
