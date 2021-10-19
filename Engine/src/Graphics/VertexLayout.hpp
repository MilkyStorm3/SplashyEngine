#pragma once
#include <vector>
#include <stdint.h>
namespace ant
{

    enum class AttributeType
    {
        null = 0,
        vec1f,
        vec2f,
        vec3f,
        vec4f,
        vec1ui,
        vec2ui,
        vec3ui,
        vec4ui
    };

    class VertexBufferLayout
    {
        using StorageType = std::vector<AttributeType>;

    public:
        VertexBufferLayout()
        {
            m_layoutTypes.reserve(3);
        }

        ~VertexBufferLayout() {}

        VertexBufferLayout(std::initializer_list<AttributeType> args);

        void SetAttribPtrs();
        void DisablePtrs();

        inline void PushAttribute(AttributeType attribute)
        {
            m_layoutTypes.push_back(attribute);
            CalcVertexSize();
        }

    private:
        void CalcVertexSize();

        uint32_t GetAttribTypeSize(AttributeType type) const;
        inline uint32_t GetAttribGlType(AttributeType type) const;
        inline uint32_t GetAttribPrimitiveTypeSize(AttributeType type) const;
        inline uint32_t GetAttribTypeComponentCount(AttributeType type) const;

    private:
        StorageType m_layoutTypes;
        uint32_t m_vertexSize = 0;
    };

} // namespace ant
