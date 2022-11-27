#include "EditorLayer.hpp"
#include <imgui.h>
#include "Core/Application.hpp"
#include "Render/Renderer.hpp"
#include "Render/RendererCommands.hpp"

#include "Graphics/Shader.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Utilities/Instrumentation.hpp"
#include "Input/Input.hpp"
#include <Core/Random.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Editor
{
    static void DockSpace()
    {
        ImGuiIO &io = ImGui::GetIO();
        static bool p_open = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &p_open, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        // DockSpace
        CORE_ASSERT(io.ConfigFlags & ImGuiConfigFlags_DockingEnable, "Imgui docking is disabled!");

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        if (ImGui::BeginMenuBar())
        {

            if (ImGui::BeginMenu("Options"))
            {
                ImGui::Separator();

                if (ImGui::MenuItem("ImGui: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, true))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Close", NULL, false, true))
                    ant::Application::GetInstance()->Terminate();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

    struct Vertex //! temp
    {
        glm::vec3 pos;
        glm::vec3 color;

        Vertex() {}

        Vertex(float x, float y, float z)
        {
            pos = glm::vec3(x, y, z);

            float red = ant::Random::Float();
            float green = ant::Random::Float();
            float blue = ant::Random::Float();
            color = glm::vec3(red, green, blue);
        }
    };

    void EditorLayer::OnAttach()
    {
        ant::Random::Init();
        CORE_TRACE("Editor Layer Attached");

        auto size = ant::Application::GetInstance()->GetActiveWindow().GetSize();

        ant::FramebufferSpecification spec = {
            size.x, size.y, {ant::FramebufferTextureFormat::RGBA8, ant::FramebufferTextureFormat::DEPTH24STENCIL8}};

        m_framebuffer = ant::FrameBuffer::Create(spec);

        m_shader = ant::Shader::Create("assets/shaders/cubeShader.glsl", true, false);
        m_lineShader = ant::Shader::Create("assets/shaders/lineShader.glsl", true, false);

        ant::RendererCommands::SetBlendingMode(ant::BlendingMode::SourceAlpha, ant::BlendingMode::OneMinusSourceAlpha);

        float points[] = {
            1.f, 1.f, 0.5f,      1.f, 0.f, 0.f,
            -1.f, -1.f, 0.5f,    1.f, 0.f, 0.f
        };

        auto pointsVB = ant::VertexBuffer::Create();
        pointsVB->GetLayout()->PushAttribute(ant::AttributeType::vec3f);
        pointsVB->GetLayout()->PushAttribute(ant::AttributeType::vec3f);
        pointsVB->UploadData((float *)&points[0], sizeof(points));

        uint32_t indx[] = {
            0, 1
        };
        auto lib = ant::IndexBuffer::Create();
        lib->UploadData(&indx[0], sizeof(indx));
        
        m_linePoints = ant::VertexArray::Create();
        m_linePoints->AddVertexBuffer(pointsVB);
        m_linePoints->SetIndexBuffer(lib);        

        Vertex vertices[8];

        vertices[0] = Vertex(0.5f, 0.5f, 0.5f);
        vertices[1] = Vertex(-0.5f, 0.5f, -0.5f);
        vertices[2] = Vertex(-0.5f, 0.5f, 0.5f);
        vertices[3] = Vertex(0.5f, -0.5f, -0.5f);
        vertices[4] = Vertex(-0.5f, -0.5f, -0.5f);
        vertices[5] = Vertex(0.5f, 0.5f, -0.5f);
        vertices[6] = Vertex(0.5f, -0.5f, 0.5f);
        vertices[7] = Vertex(-0.5f, -0.5f, 0.5f);

        auto vb = ant::VertexBuffer::Create();
        vb->GetLayout()->PushAttribute(ant::AttributeType::vec3f);
        vb->GetLayout()->PushAttribute(ant::AttributeType::vec3f);

        vb->UploadData((float *)&vertices[0], sizeof(vertices));

        auto ib = ant::IndexBuffer::Create();

        uint32_t indices[] = {
            0, 1, 2,
            1, 3, 4,
            5, 6, 3,
            7, 3, 6,
            2, 4, 7,
            0, 7, 6,
            0, 5, 1,
            1, 5, 3,
            5, 0, 6,
            7, 4, 3,
            2, 1, 4,
            0, 2, 7};

        ib->UploadData(&indices[0], sizeof(indices));

        m_vertexArray = ant::VertexArray::Create();
        m_vertexArray->AddVertexBuffer(vb);
        m_vertexArray->SetIndexBuffer(ib);

        m_camera = ant::MakeRef<EditorCamera>();
        m_camera->SetProjection(45.f, 0.1f, 100.f);
        m_camera->Resize(size);
        m_camera->SetPosition({0.f, 0.f, 4.f});

        m_projectionBuffer = ant::UniformBuffer::Create(sizeof(glm::mat4), 0);
        m_cubeTransformBuffer = ant::UniformBuffer::Create(sizeof(glm::mat4), 1);
    }

    void EditorLayer::OnUpdate(ant::TimeStep ts)
    {
        DockSpace();

        glm::ivec2 viewportBounds[2];

        {
            ImGui::Begin("Viewport");
            // ant::ImGuiLayer::SetEventBlocking(!(ImGui::IsWindowFocused() && ImGui::IsWindowHovered())); //todo
            m_camera->focused = ImGui::IsWindowFocused(); // && ImGui::IsWindowHovered();

            static glm::vec2 viewportPanelSize = {0, 0};

            auto currentViewportPanelSize = ImGui::GetContentRegionAvail();

            if (viewportPanelSize != *(glm::vec2 *)&currentViewportPanelSize.x)
            {
                viewportPanelSize = *(glm::vec2 *)&currentViewportPanelSize.x;

                if (viewportPanelSize.x > 3 && viewportPanelSize.y > 3)
                {
                    m_framebuffer->Resize(viewportPanelSize.x, viewportPanelSize.y);
                    m_camera->Resize(viewportPanelSize);
                }
            }

            ImGui::Image((void *)(m_framebuffer->GetColorAttachmentRendererId(0)), *(ImVec2 *)&viewportPanelSize.x, ImVec2{0, 1}, ImVec2{1, 0});

            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset = ImGui::GetWindowPos();
            viewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
            viewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

            ImGui::End();
        }

        glm::mat4 cubeTransform = glm::mat4(1.f);
        static float angle = 0.f;
        angle += 1.f;

        cubeTransform = glm::rotate(cubeTransform, glm::radians(angle), glm::vec3(0, 1, 0));

        ImGui::Begin("test");

        ImGui::Text("angle: %f", angle);

        auto [mx, my] = ImGui::GetMousePos();
        mx -= viewportBounds[0].x;
        my -= viewportBounds[0].y;
        glm::vec2 viewportSize = viewportBounds[1] - viewportBounds[0];
        my = viewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;
        ImGui::Text("Frametime: %fms", ts.MilliSeconds());
        ImGui::NewLine();
        ImGui::Text("Mouse in viewport x:%i y:%i", mouseX, mouseY);
        ImGui::NewLine();

        m_camera->OnUpdate(ts);
        m_projectionBuffer->SetData(&m_camera->GetCameraMatrix(), sizeof(glm::mat4));
        m_cubeTransformBuffer->SetData(&cubeTransform, sizeof(glm::mat4));

        ImGui::Text("IsFocued: %s", ImGui::IsWindowFocused() ? "true" : "false");
        ImGui::Text("IsHovered: %s", ImGui::IsWindowHovered() ? "true" : "false");

        ImGui::NewLine();
        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_W))
            ImGui::Text("W pressed");

        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_A))
            ImGui::Text("A pressed");

        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_S))
            ImGui::Text("S pressed");

        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_D))
            ImGui::Text("D pressed");

        static float thickness = 1.f;
        if(ImGui::DragFloat("Thickness",&thickness,0.003,0.1,10)){
            ant::RendererCommands::SetLineThickness(thickness);
        }
        
        ImGui::End();
    }

    void EditorLayer::OnDraw()
    {
        m_framebuffer->Bind();

        // ant::RendererCommands::Clear({1.f, 0.f, 1.f, 1.f}); // magenta
        ant::RendererCommands::Clear({0.145f, 0.156f, 0.419f, 1.f});

        ant::RendererCommands::DrawIndexed<ant::DrawMode::Lines>(m_lineShader, m_linePoints);
        ant::RendererCommands::DrawIndexed<ant::DrawMode::Triangles>(m_shader, m_vertexArray);

        m_framebuffer->UnBind();
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnEvent(ant::Event *event)
    {
    }

}