#include "OpenGlVertexLayout.hpp"
#include <Utilities/InstrumentationMacros.hpp>
#include <Gl.h>

namespace ant::OpenGl
{
    GlVertexLayout::GlVertexLayout()
    {
        m_layoutTypes.reserve(3);
    }

    inline uint32_t GlVertexLayout::GetAttribGlType(AttributeType type) const
    {
        if (uint32_t(type) <= 4)
            return GL_FLOAT;

        if (uint32_t(type) <= 8 && uint32_t(type) > 4)
            return GL_UNSIGNED_INT;

        return -1;
    }

    inline uint32_t GlVertexLayout::GetAttribPrimitiveTypeSize(AttributeType type) const
    {
        if (uint32_t(type) <= 4)
            return sizeof(float);

        if (uint32_t(type) <= 8 && uint32_t(type) > 4)
            return sizeof(uint32_t);

        return -1;
    }

    uint32_t GlVertexLayout::GetAttribTypeSize(AttributeType type) const
    {
        return GetAttribTypeComponentCount(type) * GetAttribPrimitiveTypeSize(type);
    }

    inline uint32_t GlVertexLayout::GetAttribTypeComponentCount(AttributeType type) const
    {
        uint32_t mod = uint32_t(type) % 5;

        if (mod)
            return mod;
        else
            return mod + 1;

        return -1;
    }

    GlVertexLayout::~GlVertexLayout()
    {
    }

    void GlVertexLayout::Set(std::initializer_list<AttributeType> args)
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        m_layoutTypes.clear();
        m_layoutTypes = args;
        CalcVertexSize();
    }

    void GlVertexLayout::Enable()
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        uint32_t pointerVal = 0;
        for (size_t i = 0; i < m_layoutTypes.size(); i++)
        {
            auto &ref = m_layoutTypes.at(i);
            glVertexAttribPointer(i, GetAttribTypeComponentCount(ref), GetAttribGlType(ref), GL_FALSE, m_vertexSize, (void *)pointerVal);
            glEnableVertexAttribArray(i);
            pointerVal += GetAttribTypeSize(ref);
        }
    }

    void GlVertexLayout::Disable()
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        for (size_t i = 0; i < m_layoutTypes.size(); i++)
            glDisableVertexAttribArray(i);
    }

    void GlVertexLayout::PushAttribute(AttributeType attribute)
    {        
        m_layoutTypes.push_back(attribute);
        CalcVertexSize();
    }

    void GlVertexLayout::CalcVertexSize()
    {
        m_vertexSize = 0;
        for (auto &it : m_layoutTypes)
            m_vertexSize += GetAttribTypeSize(it);
    }
}