#include "OpenGlVertexLayout.hpp"
#include <Utilities/InstrumentationMacros.hpp>
#include <Gl.h>

namespace ant::OpenGl
{
    namespace Utils
    {
        static uint32_t GetComponentCount(AttributeType type){
            switch (type)
            {
            case AttributeType::vec1f : return 1; break;
            case AttributeType::vec2f : return 2; break;
            case AttributeType::vec3f : return 3; break;
            case AttributeType::vec4f : return 4; break;
            case AttributeType::vec1ui : return 1; break;
            case AttributeType::vec2ui : return 2; break;
            case AttributeType::vec3ui : return 3; break;
            case AttributeType::vec4ui : return 4; break;
            
            default:
                CORE_ASSERT(false, "invalid enum");
                break;
            }
        }
        
    } // namespace Utils
    
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

    uint32_t GlVertexLayout::GetAttribTypeSize(AttributeType type) const
    {
        switch (type)
        {
        case AttributeType::vec1f: return sizeof(float)*1;break;
        case AttributeType::vec2f: return sizeof(float)*2;break;
        case AttributeType::vec3f: return sizeof(float)*3;break;
        case AttributeType::vec4f: return sizeof(float)*4;break;

        case AttributeType::vec1ui: return sizeof(uint32_t)*1;break;
        case AttributeType::vec2ui: return sizeof(uint32_t)*2;break;
        case AttributeType::vec3ui: return sizeof(uint32_t)*3;break;
        case AttributeType::vec4ui: return sizeof(uint32_t)*4;break;
        
        default:
            CORE_ASSERT(false, "invalid enum");
            return 0;
            break;
        }
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

    void GlVertexLayout::Enable() const
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        uint32_t pointerVal = 0;
        for (size_t i = 0; i < m_layoutTypes.size(); i++)
        {
            auto &ref = m_layoutTypes.at(i);
            glVertexAttribPointer(i, Utils::GetComponentCount(ref), GetAttribGlType(ref), GL_FALSE, m_vertexSize, (void *)pointerVal);
            glEnableVertexAttribArray(i);
            pointerVal += GetAttribTypeSize(ref);
        }
    }

    void GlVertexLayout::Disable() const
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