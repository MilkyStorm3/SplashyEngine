#include "Pch.h"
#include "Graphics/VertexLayout.hpp"

#include <Gl.h>

namespace ant
{
    inline uint32_t VertexBufferLayout::GetAttribGlType(AttributeType type) const
    {
        if (uint32_t(type) <= 4)
            return GL_FLOAT;

        if (uint32_t(type) <= 8 && uint32_t(type) > 4)
            return GL_UNSIGNED_INT;

        return -1;
    }

    inline uint32_t VertexBufferLayout::GetAttribPrimitiveTypeSize(AttributeType type) const
    {
        if (uint32_t(type) <= 4)
            return sizeof(float);

        if (uint32_t(type) <= 8 && uint32_t(type) > 4)
            return sizeof(uint32_t);

        return -1;
    }

    uint32_t VertexBufferLayout::GetAttribTypeSize(AttributeType type) const
    {
        return GetAttribTypeComponentCount(type) * GetAttribPrimitiveTypeSize(type);
    }

    inline uint32_t VertexBufferLayout::GetAttribTypeComponentCount(AttributeType type) const
    {
        uint32_t mod = uint32_t(type) % 5;

        if (mod)
            return mod;
        else
            return mod + 1;

        return -1;
    }

    VertexBufferLayout::VertexBufferLayout(std::initializer_list<AttributeType> args)
    {
        m_layoutTypes = args;
        CalcVertexSize();
    }

    void VertexBufferLayout::Enable()
    {
        CORE_PROFILE_FUNC();
        uint32_t pointerVal = 0;
        for (size_t i = 0; i < m_layoutTypes.size(); i++)
        {
            auto &ref = m_layoutTypes.at(i);
            glVertexAttribPointer(i, GetAttribTypeComponentCount(ref), GetAttribGlType(ref), GL_FALSE, m_vertexSize, (void *)pointerVal);
            glEnableVertexAttribArray(i);
            pointerVal += GetAttribTypeSize(ref);
        }
    }

    void VertexBufferLayout::Disable()
    {
        for (size_t i = 0; i < m_layoutTypes.size(); i++)
            glDisableVertexAttribArray(i);
    }

    void VertexBufferLayout::CalcVertexSize()
    {
        m_vertexSize = 0;
        for (auto &it : m_layoutTypes)
            m_vertexSize += GetAttribTypeSize(it);
    }

} // namespace ant