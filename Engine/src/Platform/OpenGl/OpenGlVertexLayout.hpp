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

        const std::vector<AttributeType> &GetRawAttribs() const { return m_layoutTypes; }

        void SetVertexSize(uint32_t size){m_vertexSize = size;}

    private:
        std::vector<AttributeType> m_layoutTypes;
        uint32_t m_vertexSize = 0;
    };
} // namespace ant::OpenGl
