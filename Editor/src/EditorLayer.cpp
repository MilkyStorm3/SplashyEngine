#include "EditorLayer.hpp"
#include <imgui/imgui.h>

namespace Editor
{
    void EditorLayer::OnAttach() 
    {
        CORE_INFO("Editor Layer Attached");
    }
    
    void EditorLayer::OnUpdate() 
    {
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
}