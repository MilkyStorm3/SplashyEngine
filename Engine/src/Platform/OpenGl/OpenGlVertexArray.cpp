#include "OpenGlVertexArray.hpp"
#include <GL/glew.h> 
#include "OpenGlVertexBuffer.hpp"
#include <Utilities/InstrumentationMacros.hpp>

namespace ant::OpenGl
{

    namespace Utils
    {

        static uint32_t GetCompomentCount(AttributeType type)
        {

            switch (type)
            {
            case AttributeType::vec1f:
                return 1;
                break;
            case AttributeType::vec2f:
                return 2;
                break;
            case AttributeType::vec3f:
                return 3;
                break;
            case AttributeType::vec4f:
                return 4;
                break;
            case AttributeType::vec1ui:
                return 1;
                break;
            case AttributeType::vec2ui:
                return 2;
                break;
            case AttributeType::vec3ui:
                return 3;
                break;
            case AttributeType::vec4ui:
                return 4;
                break;

            default:
                CORE_ASSERT(false, "invalid enum");
                break;
            }
            return 0;
        }

        static GLenum GetGlType(AttributeType type)
        {

            switch (type)
            {
            case AttributeType::vec1f:
                return GL_FLOAT;
                break;
            case AttributeType::vec2f:
                return GL_FLOAT;
                break;
            case AttributeType::vec3f:
                return GL_FLOAT;
                break;
            case AttributeType::vec4f:
                return GL_FLOAT;
                break;

            case AttributeType::vec1ui:
                return GL_UNSIGNED_INT;
                break;
            case AttributeType::vec2ui:
                return GL_UNSIGNED_INT;
                break;
            case AttributeType::vec3ui:
                return GL_UNSIGNED_INT;
                break;
            case AttributeType::vec4ui:
                return GL_UNSIGNED_INT;
                break;

            default:
                CORE_ASSERT(false, "invalid enum");
                break;
            }
            return GL_INVALID_ENUM;
        }

        static uint32_t GetAttribSize(AttributeType type)
        {

            switch (type)
            {
            case AttributeType::vec1f:
                return sizeof(float) * 1;
                break;
            case AttributeType::vec2f:
                return sizeof(float) * 2;
                break;
            case AttributeType::vec3f:
                return sizeof(float) * 3;
                break;
            case AttributeType::vec4f:
                return sizeof(float) * 4;
                break;

            case AttributeType::vec1ui:
                return sizeof(uint32_t) * 1;
                break;
            case AttributeType::vec2ui:
                return sizeof(uint32_t) * 2;
                break;
            case AttributeType::vec3ui:
                return sizeof(uint32_t) * 3;
                break;
            case AttributeType::vec4ui:
                return sizeof(uint32_t) * 4;
                break;

            default:
                CORE_ASSERT(false, "invalid enum");
                break;
            }
            return 0;
        }

    }

    GlVertexArray::GlVertexArray()
    {
        CORE_DETAILED_PROFILE_FUNC();
        glCreateVertexArrays(1, &m_glId);
    }

    GlVertexArray::~GlVertexArray()
    {
        CORE_DETAILED_PROFILE_FUNC();
        glDeleteVertexArrays(1, &m_glId);
    }

    void GlVertexArray::Bind() const
    {
        glBindVertexArray(m_glId);
    }

    void GlVertexArray::AddVertexBuffer(Ref<VertexBuffer> vertexBuffer)
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        //! if new layout attrib is added GlLayout is not updated
        Ref<GlVertexLayout> layout = std::static_pointer_cast<GlVertexLayout>(vertexBuffer->GetLayout());

        size_t vertexBufferIndex = m_vertexBuffers.size();
        auto &layoutAttribs = layout->GetRawAttribs();

        uint32_t offsetVal = 0;

        for (size_t i = 0; i < layoutAttribs.size(); i++)
        {
            auto &&attrib = layoutAttribs.at(i);

            glEnableVertexArrayAttrib(m_glId, i);

            glVertexArrayAttribBinding(m_glId, i, vertexBufferIndex);
            glVertexArrayAttribFormat(m_glId, i, Utils::GetCompomentCount(attrib), Utils::GetGlType(attrib), GL_FALSE, offsetVal);

            offsetVal += Utils::GetAttribSize(attrib);
        }

        uint32_t bufferId = vertexBuffer->GetRendererId();
        glVertexArrayVertexBuffer(m_glId, vertexBufferIndex, bufferId, 0, offsetVal); //? check what is that offset parameter set to 0??

        layout->SetVertexSize(offsetVal);
        m_vertexBuffers.push_back(vertexBuffer);
    }

    void GlVertexArray::SetIndexBuffer(Ref<IndexBuffer> indexBuffer)
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        m_indexBuffer = indexBuffer;
        uint32_t indexBufferId = m_indexBuffer->GetRendererId();
        glVertexArrayElementBuffer(m_glId, indexBufferId);
    }

    const std::vector<Ref<VertexBuffer>> &GlVertexArray::GetVertexBuffers() const
    {
        return m_vertexBuffers;
    }

    const Ref<IndexBuffer> &GlVertexArray::GetIndexBuffer() const
    {
        return m_indexBuffer;
    }
}