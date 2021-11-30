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

        /**
         * @brief  Enables given vertex layout in opengl calls glVertexAttribPointer(idx) and glEnableVertexAttribArray(idx)
         * @retval None
         */
        void Enable();

        /**
         * @brief  Disables every single attribute by calling glDisableVertexAttribArray(idx);
         * @retval None
         */
        void Disable();

        /**
         * @brief Add new vertex attribute to the list
         * @param  attribute: Type of attribute
         */
        inline void PushAttribute(AttributeType attribute)
        {
            m_layoutTypes.push_back(attribute);
            CalcVertexSize();
        }

        /**
         * @return Size of single vertex for given layout (RAW BYTES)
         */
        inline uint32_t GetVertexSize() const { return m_vertexSize; }

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
