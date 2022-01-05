#include "EditorLayer.hpp"
#include <imgui/imgui.h>
#include "Core/Application.hpp"
#include "Render/Primitive.hpp"
#include "Render/Renderer.hpp"

namespace Editor
{
    void EditorLayer::OnAttach() 
    {
        CORE_INFO("Editor Layer Attached");

        auto size = ant::Application::GetInstance()->GetWindow().GetSize();
        // m_framebuffer = ant::FrameBuffer::Create(size.x, size.y);


    }
    
    void EditorLayer::OnUpdate() 
    {
        DockSpace();


         {
            ImGui::Begin("Viewport");
            // ant::ImGuiLayer::SetEventBlocking(!(ImGui::IsWindowFocused() && ImGui::IsWindowHovered())); //todo

            static glm::vec2 viewportPanelSize = {0, 0};

            auto currentViewportPanelSize = ImGui::GetContentRegionAvail();

            if (viewportPanelSize != *(glm::vec2 *)&currentViewportPanelSize.x)
            {
                viewportPanelSize = *(glm::vec2 *)&currentViewportPanelSize.x;

                CORE_WARN("PanelSize: x - {0}, y - {1}", viewportPanelSize.x, viewportPanelSize.y);
                if (viewportPanelSize.x > 3 && viewportPanelSize.y > 3)
                {
                    //? m_framebuffer->Resize(viewportPanelSize.x, viewportPanelSize.y);
                    // m_cameraController->OnResize(viewportPanelSize.x, viewportPanelSize.y);
                    //todo resize the camera and frame buffer
                }
            }
            // move
            //? ImGui::Image((void *)m_framebuffer->GetGlId(), *(ImVec2 *)&viewportPanelSize.x, ImVec2{0, 1}, ImVec2{1, 0});

            ImGui::End();
        }

        ImGui::Begin("test");

        ImGui::Text("welcome back babe!");

        ImGui::End();

    }
    
    void EditorLayer::OnDraw() 
    {
        // m_framebuffer->Bind();
        // static auto q = ant::MakeUniqueRef<ant::OldQuad>();

        // q->SetColor({0.8,0.2,0.2,1.0});

        // ant::Renderer2D::BeginScene();
        // ant::Renderer2D::DrawQuad(*q);
        // ant::Renderer2D::EndScene();

        // ant::FrameBuffer::BindDefault();



        

    }
    
    void EditorLayer::OnDetach() 
    {
        
    }
    
    void EditorLayer::OnEvent(ant::Event *event) 
    {
        
    }
    
    void EditorLayer::DockSpace() 
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
}   