#include "OpenGlVertexLayout.hpp"
#include <Utilities/InstrumentationMacros.hpp>
#include <GL/glew.h>

namespace ant::OpenGl
{

    GlVertexLayout::GlVertexLayout()
    {
        m_layoutTypes.reserve(3);
    }

    GlVertexLayout::~GlVertexLayout()
    {
    }

    void GlVertexLayout::Set(std::initializer_list<AttributeType> args)
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        m_layoutTypes.clear();
        m_layoutTypes = args;
    }

    void GlVertexLayout::Enable() const
    {
        CORE_ASSERT(false, "NOT IMPLEMENTED!");
    }

    void GlVertexLayout::Disable() const
    {
        CORE_ASSERT(false, "NOT IMPLEMENTED!");
    }

    void GlVertexLayout::PushAttribute(AttributeType attribute)
    {
        m_layoutTypes.push_back(attribute);
    }

}