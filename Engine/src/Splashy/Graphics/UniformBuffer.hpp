#pragma once
#include "Core/Core.hpp"

namespace ant
{

    class UniformBuffer
    {
    public:
        UniformBuffer() {}
        virtual ~UniformBuffer() {}

        static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);

        virtual void SetData(const void *data, uint32_t size, uint32_t offset = 0) = 0;
    };

} // namespace ant
