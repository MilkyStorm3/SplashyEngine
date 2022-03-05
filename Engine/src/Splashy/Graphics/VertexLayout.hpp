#pragma once
#include <initializer_list>
#include <Core/Core.hpp>
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

    class VertexLayout
    {
    public:
        static Ref<VertexLayout> Create();

        VertexLayout(){};
        virtual ~VertexLayout() {}
        VertexLayout(const VertexLayout &) = delete;

        // VertexBufferLayout(std::initializer_list<AttributeType> args);

        virtual void Set(std::initializer_list<AttributeType> args) = 0;

        /**
         * @brief  Enables given vertex layout in opengl calls glVertexAttribPointer(idx) and glEnableVertexAttribArray(idx)
         * @retval None
         */
        virtual void Enable() const = 0;

        /**
         * @brief  Disables every single attribute by calling glDisableVertexAttribArray(idx);
         * @retval None
         */
        virtual void Disable() const = 0;

        /**
         * @brief Add new vertex attribute to the list
         * @param  attribute: Type of attribute
         */
        virtual void PushAttribute(AttributeType attribute) = 0;

        /**
         * @return Size of single vertex for given layout (RAW BYTES)
         */
        virtual uint32_t GetVertexSize() const = 0;

    };

} // namespace ant
