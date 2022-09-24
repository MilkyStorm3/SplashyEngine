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

    void EditorLayer::OnAttach()
    {
        CORE_TRACE("Editor Layer Attached");

        auto size = ant::Application::GetInstance()->GetWindow().GetSize();

        ant::RendererCommands::SetBlendingMode(ant::BlendingMode::SourceAlpha, ant::BlendingMode::OneMinusSourceAlpha);

        m_imageData = new uint32_t[1000000]; // BIG buffer of pixels
    }

    void EditorLayer::OnUpdate(ant::TimeStep ts)
    {
        DockSpace();

        glm::ivec2 viewportBounds[2];

        {
            ImGui::Begin("Viewport");

            static glm::vec2 viewportPanelSize = {0, 0};

            auto currentViewportPanelSize = ImGui::GetContentRegionAvail();

            if (!m_imageTexture)
            {
                m_imageTexture = ant::Texture2D::Create(currentViewportPanelSize.x, currentViewportPanelSize.y);
            }

            for (size_t y = 0; y < currentViewportPanelSize.y; y++)
            {
                for (size_t x = 0; x < currentViewportPanelSize.x; x++)
                {
                    size_t i = x + y * currentViewportPanelSize.x;
                    float uvx = ((float)x / (float)currentViewportPanelSize.x) * 2.f - 1.f;
                    float uvy = ((float)y / (float)currentViewportPanelSize.y) * 2.f - 1.f;

                    PerPixel(m_imageData[i], uvx, uvy);
                }
            }

            if (viewportPanelSize != *(glm::vec2 *)&currentViewportPanelSize.x)
            {
                viewportPanelSize = *(glm::vec2 *)&currentViewportPanelSize.x;

                if (viewportPanelSize.x > 3 && viewportPanelSize.y > 3)
                {
                    auto [x, y] = currentViewportPanelSize;
                    m_imageTexture = ant::Texture2D::Create(x, y);
                }
            }

            auto [x, y] = currentViewportPanelSize;
            m_imageTexture->SetData(m_imageData, x * y * 4);
            ImGui::Image((void *)(m_imageTexture->GetRendererId()), *(ImVec2 *)&viewportPanelSize.x, ImVec2{0, 1}, ImVec2{1, 0});

            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset = ImGui::GetWindowPos();
            viewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
            viewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

            ImGui::End();
        }

        ImGui::Begin("test");

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

        ImGui::End();
    }

    void EditorLayer::OnDraw()
    {
    }

    void EditorLayer::OnDetach()
    {
        delete[] m_imageData;
    }

    void EditorLayer::OnEvent(ant::Event *event)
    {
    }

    void EditorLayer::PerPixel(uint32_t &pixel, float x, float y)
    {

        if (x < 0)
            x = -x;

        if (y < 0)
            y = -y;

        // pixel = 0xff00ffff;
        uint8_t r = (uint8_t)(x * 255.f);
        uint8_t g = (uint8_t)(y * 255.f);
        uint8_t b = 0x88;

        pixel = 0xff000000 | (b << 16) | (g << 8) | r;
    }

}