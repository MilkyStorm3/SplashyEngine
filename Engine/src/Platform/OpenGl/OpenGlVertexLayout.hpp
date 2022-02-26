#pragma once
#include <Graphics/VertexLayout.hpp>
#include <vector>

namespace ant::OpenGl
{
    class GlVertexLayout : public VertexLayout
    {
    public:
        GlVertexLayout();
        ~GlVertexLayout();

        virtual void Set(std::initializer_list<AttributeType> args) override;

        virtual void Enable() const override;
        virtual void Disable() const override;

        virtual void PushAttribute(AttributeType attribute) override;
        virtual uint32_t GetVertexSize() const override { return m_vertexSize; }

    private:
        void CalcVertexSize();

    private:
        uint32_t GetAttribTypeSize(AttributeType type) const;
        inline uint32_t GetAttribGlType(AttributeType type) const;
        // inline uint32_t GetAttribPrimitiveTypeSize(AttributeType type) const;
        // inline uint32_t GetAttribTypeComponentCount(AttributeType type) const;

        std::vector<AttributeType> m_layoutTypes;
        uint32_t m_vertexSize = 0;
    };
} // namespace ant::OpenGl
