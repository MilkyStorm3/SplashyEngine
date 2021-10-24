#pragma once
#ifdef PRIMITIVES


#include "Graphics/Buffer.hpp"
#include "Render/Transform.hpp"
#include "Graphics/Texture.hpp"

namespace ant
{

    struct Vertex
    {
        glm::vec4 position;
        glm::vec4 color;
        glm::vec2 textureCoordinate;
        float textureId = 0;
        static VertexBufferLayout layout;
        operator float *();
        #define INT_VERTEX_LAYOUT_DECL VertexBufferLayout Vertex::layout = {AttributeType::vec4f, AttributeType::vec4f, AttributeType::vec2f, AttributeType::vec1f};
    };

    class OldQuad
        : public TransformComponent
    {
    public:
        OldQuad();  
        ~OldQuad() {}

        friend class Renderer2D;
        friend class Renderer2DQueue;

        void SetColor(const glm::vec4 &color);
        void SetTexture(Ref<Texture> texture);
        Ref<Texture> GetTexture() const { return m_texture; }
        inline const glm::vec4 &GetColor() const { return m_color; }

        inline void SetSubTexture(Ref<SubTexture> tex) { SetSubTexture(*tex); }
        void SetSubTexture(const SubTexture &tex);

    private:
        void SetTexId(uint32_t id);
        glm::vec4 m_color = {1.f, 1.f, 1.f, 1.f};
        Ref<Texture> m_texture;
        std::array<Vertex, 4> m_vertices;
        std::array<uint32_t, 6> m_indices;
    };

    class Quad
    {   
    public:
        friend class Renderer2D;
        friend class Renderer2DQueue;

        Quad(const glm::vec4 &color = {1.f, 1.f, 1.f, 1.f});
        ~Quad() {}

        inline const glm::vec4 &GetColor() const { return m_color; }
        inline glm::vec4 &GetColorRef() { return m_color; }
        
        void SetColor(const glm::vec4 &color);
        void UpdateColor();

    private:
        glm::vec4 m_color;

        std::array<Vertex, 4> m_vertices;
        static const std::array<uint32_t, 6> s_indices;
    };

}
#endif