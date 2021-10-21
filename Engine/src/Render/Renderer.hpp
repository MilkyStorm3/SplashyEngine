#pragma once
#include <glm/glm.hpp>
#include "Graphics/Buffer.hpp"
#include "Graphics/Shader.hpp"
#include "Render/Primitive.hpp"
#include "Graphics/Texture.hpp"
#include "Camera/Camera.hpp"
#include "Graphics/FrameBuffer.hpp"
#include "Scene/Components.hpp"
namespace ant
{

    class Renderer2DQueue
    {
    public:
        ~Renderer2DQueue() {}

    private:
        friend class Renderer2D;
        Renderer2DQueue();

        void Add(OldQuad &shape);
        void Add(Quad &shape, TransformComponent& transform);

        std::pair<float *, size_t> GetData();
        std::pair<uint32_t *, size_t> GetIdxData();
   
    private:
        uint32_t m_objectCount = 0;
        uint32_t m_verticesCount = 0;
        uint32_t m_indicesCount = 0;

        static constexpr size_t quadsLimit = 1000;   
        using VertexStorage = std::array<Vertex, 4 * quadsLimit>;
        using IndexStorage = std::array<uint32_t, 6 * quadsLimit>;
        Ref<VertexStorage> m_vertices;
        Ref<IndexStorage> m_indices;
    };

    class Renderer2D
    {
    private:
        friend class Renderer2DQueue;

        struct SceneData
        {
            Ref<Shader> shader = Shader::Create("shaders/Shader.glsl");
            // Ref<Shader> shader;
            Ref<OrthographicCamera> camera; 
            Ref<Texture> defaultTexture;
            Renderer2DQueue queue;

            struct TexturesData
            {
                uint32_t count = 1;
                uint32_t slotLimit = 32;
                std::unordered_map<Texture *, bool> usedTextures;
            } textures;
        };
        struct RendererStats
        {
            uint32_t drawCallsCount = 0;
            uint32_t shapesCount = 0;
            uint32_t verticesCount = 0;
            uint32_t indicesCount = 0;

            void Reset()
            {
                drawCallsCount = 0;
                shapesCount = 0;
                verticesCount = 0;
                indicesCount = 0;
            }
        };

    public:
        static void Init();
        static void OnUpdate();
        static void BeginScene(Ref<OrthographicCamera> camera, Ref<FrameBuffer> drawTarget = nullptr);

        // static void DrawShape(Shape &shape);
        static void DrawQuad(OldQuad &shape);
        static void DrawQuad(Quad &shape, TransformComponent& transform);
        static void DrawTexturedQuad(Quad &shape, TransformComponent& transform, TextureComponent& texture);

        static void EndScene();

        static void DrawIndexed(Ref<Material> material, VertexArrayPrimitive &vertexArray);
        static void DrawIndexed(Ref<Shader> shader, VertexArrayPrimitive &vertexArray);

        static RendererStats GetStats() { return s_stats; }

    private:
        Renderer2D() {}
        ~Renderer2D() {} 
        static void EndBatch();

    private:
        static SceneData s_sceneData;
        static RendererStats s_stats;
    };

} // namespace ant
