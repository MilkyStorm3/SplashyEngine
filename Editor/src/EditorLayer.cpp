#include "EditorLayer.hpp"
#include <imgui/imgui.h>
#include "Core/Application.hpp"
namespace Editor
{
    void EditorLayer::OnAttach() 
    {
        CORE_INFO("Editor Layer Attached");
    }
    
    void EditorLayer::OnUpdate() 
    {
        DockSpace();
        
        ImGui::Begin("test");

        ImGui::Text("welcome back babe!");

        ImGui::End();
    }
    
    void EditorLayer::OnDraw() 
    {
        
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