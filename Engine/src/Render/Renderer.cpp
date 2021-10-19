
#include "Pch.h"

#include "Render/Renderer.hpp"
#include "Graphics/FrameBuffer.hpp"

#include <Gl.h>

namespace ant
{
    Renderer2D::SceneData Renderer2D::s_sceneData;
    Renderer2D::RendererStats Renderer2D::s_stats;

    void RendererCommands::SetClearColor(glm::vec4 color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RendererCommands::Clear()
    {
        CORE_PROFILE_FUNC();
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    Renderer2DQueue::Renderer2DQueue()
    {
    }

    void Renderer2DQueue::Add(OldQuad &shape)
    {
        CORE_PROFILE_FUNC();
        auto vsize = shape.m_vertices.size();
        auto vptr = &shape.m_vertices[0];

        shape.CalculateTranformationMatrix();
        auto &mat = shape.GetTransformationMatrix();

        for (size_t i = 0; i < vsize; i++)
        {
            Vertex vertex = vptr[i];
            vertex.position = mat * vertex.position;
            m_vertices->at(m_verticesCount) = vertex;
            Renderer2D::s_stats.verticesCount++;
            m_verticesCount++;
        }

        auto iptr = &shape.m_indices[0];
        auto isize = shape.m_indices.size();

        for (size_t i = 0; i < isize; i++)
        {
            uint32_t idx = iptr[i] + (m_objectCount * vsize);
            m_indices->at(m_indicesCount) = idx;
            Renderer2D::s_stats.indicesCount++;
            m_indicesCount++;
        }
        m_objectCount++;

        Renderer2D::s_stats.shapesCount++;
    }

    void Renderer2DQueue::Add(Quad &shape, TransformComponent &transform)
    {
        CORE_PROFILE_FUNC();
        auto vsize = shape.m_vertices.size();
        auto vptr = &shape.m_vertices[0];

        transform.CalculateTranformationMatrix();
        auto &mat = transform.GetTransformationMatrix();

        for (size_t i = 0; i < vsize; i++)
        {
            Vertex vertex = vptr[i];
            vertex.position = mat * vertex.position;
            m_vertices->at(m_verticesCount) = vertex;
            Renderer2D::s_stats.verticesCount++;
            m_verticesCount++;
        }

        auto iptr = &shape.s_indices[0];
        auto isize = shape.s_indices.size();

        for (size_t i = 0; i < isize; i++)
        {
            uint32_t idx = iptr[i] + (m_objectCount * vsize);
            m_indices->at(m_indicesCount) = idx;
            Renderer2D::s_stats.indicesCount++;
            m_indicesCount++;
        }
        m_objectCount++;

        Renderer2D::s_stats.shapesCount++;
    }

    std::pair<float *, size_t> Renderer2DQueue::GetData()
    {
        return {(float *)m_vertices->data(), m_verticesCount * sizeof(Vertex)};
    }

    std::pair<uint32_t *, size_t> Renderer2DQueue::GetIdxData()
    {
        return {m_indices->data(), m_indicesCount};
    }

    void Renderer2D::Init()
    {
        CORE_PROFILE_FUNC();

        s_sceneData.shader->CreateShader();
        s_sceneData.shader->BindShader();

        s_sceneData.queue.m_vertices = MakeRef<Renderer2DQueue::VertexStorage>();
        s_sceneData.queue.m_indices = MakeRef<Renderer2DQueue::IndexStorage>();

        auto tex = Texture::Create(glm::ivec2(1, 1));
        uint32_t data = 0xffffffff;
        tex->SetData(&data, sizeof(data));
        tex->Upload(0);
        tex->SetKeepLocalBuffer(true);

        s_sceneData.defaultTexture = tex;
        int arr[32];

        for (int i = 0; i < 32; i++)
            arr[i] = i;

        s_sceneData.shader->SetUniform("u_textures").SetAllowedDataType(Uniform::DataType::ivec1);
        s_sceneData.shader->SetUniform("u_textures").UploadArray(arr, 32);
    }

    void Renderer2D::OnUpdate()
    {
        s_stats.Reset();
    }

    void Renderer2D::BeginScene(Ref<OrthographicCamera> camera, Ref<FrameBuffer> drawTarget)
    {
        CORE_PROFILE_FUNC();
        s_sceneData.camera = camera;

        if (drawTarget)
        {
            drawTarget->Bind();
            RendererCommands::Clear();
        }
    }

    void Renderer2D::DrawQuad(OldQuad &shape)
    {
        CORE_PROFILE_FUNC();

        if (s_sceneData.queue.m_objectCount >= Renderer2DQueue::quadsLimit)
            EndBatch();

        if (shape.GetTexture())
        {
            auto &count = s_sceneData.textures.count;

            if (count >= s_sceneData.textures.slotLimit)
            {
                EndBatch();
                s_sceneData.textures.usedTextures.clear();
            }

            bool &act = s_sceneData.textures.usedTextures[shape.GetTexture().get()];

            if (act)
            {
                shape.SetTexId(shape.GetTexture()->GetSlot());
            }
            else
            {
                shape.GetTexture()->Upload(count);
                act = true;
                shape.SetTexId(count);
                count++;
            }
        }

        s_sceneData.queue.Add(shape);
    }

    void Renderer2D::DrawQuad(Quad &shape, TransformComponent &transform)
    {
        CORE_PROFILE_FUNC();

        if (s_sceneData.queue.m_objectCount >= Renderer2DQueue::quadsLimit)
            EndBatch();

        s_sceneData.queue.Add(shape, transform);
    }

    void Renderer2D::DrawTexturedQuad(Quad &shape, TransformComponent &transform, TextureComponent &textureComponent)
    {

        if (s_sceneData.queue.m_objectCount >= Renderer2DQueue::quadsLimit)
            EndBatch();

        auto texture = textureComponent.Texture->GetTexture();

        auto &count = s_sceneData.textures.count;

        if (count >= s_sceneData.textures.slotLimit)
        {
            EndBatch();
            s_sceneData.textures.usedTextures.clear();
        }

        bool &act = s_sceneData.textures.usedTextures[texture.get()];

        if (act)
        {
            for (auto &vertex : shape.m_vertices)
                vertex.textureId = float(texture->GetSlot()); //! todo
        }
        else
        {
            texture->Upload(count);
            act = true;

            for (auto &vertex : shape.m_vertices)
                vertex.textureId = float(count);
            count++;
        }

        //? sets a subtexture coordinates
        for (size_t i = 0; i < shape.m_vertices.size(); i++)
            shape.m_vertices[i].textureCoordinate = textureComponent.Texture->GetCoordinateData().at(i); 

        s_sceneData.queue.Add(shape, transform);
    }

    void Renderer2D::EndScene()
    {
        EndBatch();
        FrameBuffer::BindDefault();
    }

    void Renderer2D::EndBatch()
    {
        CORE_PROFILE_FUNC();
        VertexArrayPrimitive vao;

        vao.UploadData(s_sceneData.queue.GetData());
        vao.GetIndexBuffer().UploadData(s_sceneData.queue.GetIdxData());
        vao.SetLayout(Vertex::layout);
        vao.Bind();

        auto &shader = s_sceneData.shader->SetUniform("u_ViewProjectionMatrix");
        shader.SetAllowedDataType(Uniform::DataType::mat4f);
        shader = s_sceneData.camera->GetViewProjectionMatrix();

        {
            CORE_PROFILE_SCOPE("Draw call");
            glDrawElements(GL_TRIANGLES, vao.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);
        }

        s_sceneData.queue.m_objectCount = 0;
        s_sceneData.queue.m_verticesCount = 0;
        s_sceneData.queue.m_indicesCount = 0;
        s_sceneData.textures.count = 1;
        s_stats.drawCallsCount++;
    }

    void Renderer2D::DrawIndexed(Ref<Material> material, VertexArrayPrimitive &vertexArray)
    {
        material->Use();
        vertexArray.Bind();
        glDrawElements(GL_TRIANGLES, vertexArray.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer2D::DrawIndexed(Ref<Shader> shader, VertexArrayPrimitive &vertexArray)
    {
        shader->BindShader();
        vertexArray.Bind();
        glDrawElements(GL_TRIANGLES, vertexArray.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);
    }

} // namespace ant
