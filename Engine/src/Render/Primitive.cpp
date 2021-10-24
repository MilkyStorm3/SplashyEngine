#ifdef PRIMITIVES
#include "Pch.h"
#include "Render/Primitive.hpp"

namespace ant
{

    const std::array<uint32_t, 6> Quad::s_indices = {0, 1, 2, 2, 3, 0};

    Vertex::operator float *()
    {
        return &position.x;
    }

    INT_VERTEX_LAYOUT_DECL

    OldQuad::OldQuad()
    {
        m_vertices[0] = {{-0.5f, 0.5f, 0.f, 1.f}, m_color, {0.0f, 1.0f}};
        m_vertices[1] = {{-0.5f, -0.5f, 0.f, 1.f}, m_color, {0.0f, 0.0f}};
        m_vertices[2] = {{0.5f, -0.5f, 0.f, 1.f}, m_color, {1.0f, 0.0f}};
        m_vertices[3] = {{0.5f, 0.5f, 0.f, 1.f}, m_color, {1.0f, 1.0f}};

        m_indices[0] = 0;
        m_indices[1] = 1;
        m_indices[2] = 2;
        m_indices[3] = 2;
        m_indices[4] = 3;
        m_indices[5] = 0;
    }

    void OldQuad::SetColor(const glm::vec4 &color)
    {
        m_color = color;

        for (auto &vertex : m_vertices)
            vertex.color = m_color;
    }

    void OldQuad::SetTexture(Ref<Texture> texture)
    {
        m_texture = texture;
    }

    void OldQuad::SetSubTexture(const SubTexture &tex)
    {
        SetTexture(tex.GetTexture());
        for (size_t i = 0; i < m_vertices.size(); i++)
            m_vertices[i].textureCoordinate = tex.GetCoordinateData().at(i);
    }

    void OldQuad::SetTexId(uint32_t id)
    {
        for (auto &vertex : m_vertices)
            vertex.textureId = float(id);
    }

    Quad::Quad(const glm::vec4 &color)
        : m_color(color)
    {
        m_vertices[0] = {{-0.5f, 0.5f, 0.f, 1.f}, m_color, {0.0f, 1.0f}};
        m_vertices[1] = {{-0.5f, -0.5f, 0.f, 1.f}, m_color, {0.0f, 0.0f}};
        m_vertices[2] = {{0.5f, -0.5f, 0.f, 1.f}, m_color, {1.0f, 0.0f}};
        m_vertices[3] = {{0.5f, 0.5f, 0.f, 1.f}, m_color, {1.0f, 1.0f}};
    }

    void Quad::SetColor(const glm::vec4 &color)
    {
        m_color = color;

        UpdateColor();
    }

    void Quad::UpdateColor()
    {
        for (auto &vertex : m_vertices)
            vertex.color = m_color;
    }

}
#endif