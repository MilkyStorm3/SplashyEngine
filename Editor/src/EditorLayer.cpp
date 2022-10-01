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
#include <glm/glm.hpp>
#include "Input/Event.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Sandbox
{
    static void DockSpace()
    {
        ImGuiIO &io = ImGui::GetIO();
        static bool p_open = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

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

    constexpr uint32_t c_imageDataSize = 1000000;
    void EditorLayer::OnAttach()
    {
        CORE_TRACE("Editor Layer Attached");

        auto size = ant::Application::GetInstance()->GetWindow().GetSize();

        ant::RendererCommands::SetBlendingMode(ant::BlendingMode::SourceAlpha, ant::BlendingMode::OneMinusSourceAlpha);

        m_imageData = new uint32_t[c_imageDataSize]; // BIG buffer of pixels

        m_scene.spheres.reserve(2);
        m_scene.spheres.push_back({0.5f,
                                   {0, 1, 0},
                                   {1, 1, 0}});

        m_scene.spheres.push_back({0.5f,
                                   {1.3, 1, 0},
                                   {0.8, 0.2, 0.2}});
    }

    void EditorLayer::OnUpdate(ant::TimeStep ts)
    {
        DockSpace();

        glm::ivec2 viewportBounds[2];
        ImVec2 currentViewportPanelSize;

        {
            ImGui::Begin("Viewport");

            static glm::vec2 viewportPanelSize = {0, 0};

            currentViewportPanelSize = ImGui::GetContentRegionAvail();
            m_viewportFocus = ImGui::IsWindowFocused();

            if (viewportPanelSize != *(glm::vec2 *)&currentViewportPanelSize.x)
            {
                viewportPanelSize = *(glm::vec2 *)&currentViewportPanelSize.x;

                if (viewportPanelSize.x > 3 && viewportPanelSize.y > 3)
                {
                    auto [x, y] = currentViewportPanelSize;
                    m_imageTexture = ant::Texture2D::Create(x, y);

                    m_camera.SetProjection(45, 1, 100);
                    m_camera.Resize(*(glm::vec2 *)&currentViewportPanelSize[0]);
                }
            }

            auto [x, y] = currentViewportPanelSize;
            // Clear({1.f,1.f,1.f,1.f});

            m_imageTexture->SetData(m_imageData, x * y * sizeof(uint32_t));
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

        glm::vec2 uv = glm::vec2{mouseX, mouseY} / glm::vec2{currentViewportPanelSize.x, currentViewportPanelSize.y} * 2.f - 1.f;
        ImGui::Text("Mouse UV coordinates x:%f y:%f", uv.x, uv.y);

        uint32_t pixel = 0;
        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_R))
        {
            pixel = m_imageData[size_t(mouseX + mouseY * currentViewportPanelSize.x)];
        }

        ImGui::NewLine();

        ImGui::Text("Mouse color (press R key) r:%i g:%i b:%i a:%i",

                    (pixel & 0x000000ff),       // r
                    (pixel & 0x0000ff00) >> 8,  // g
                    (pixel & 0x00ff0000) >> 16, // b
                    (pixel & 0xff000000) >> 24  // a
        );

        ImGui::Separator();

        ImGui::NewLine();
        ImGui::DragFloat("cameraMovementSpeed", &m_camera.movementSpeed, 0.0001f, 0.000001f, 4.f);

        ImGui::NewLine();
        ImGui::DragFloat("cameraMouseSpeed", &m_camera.mouseSpeed, 0.0001f, 0.0001f, 3.f);

        ImGui::NewLine();
        static glm::vec3 cpos = m_camera.GetPosition();
        if (ImGui::DragFloat3("camera position", &cpos.x, 0.001f, -20.0f, 20.f))
        {
            m_camera.SetPosition(cpos);
        }

        ImGui::NewLine();
        ImGui::DragFloat3("light direction", &m_lightDirection.x, 0.001f, -20.0f, 20.f);

        ImGui::NewLine();
        ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor));

        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_B))
            ant::Input::SetCursor(ant::CursorStyle::Disabled);

        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_V))
            ant::Input::SetCursor(ant::CursorStyle::Normal);

        if (m_viewportFocus)
            m_camera.OnUpdate(ts);

        ImGui::Separator();
        ImGui::NewLine();

        for (size_t i = 0; i < m_scene.spheres.size(); i++)
        {
            Sphere &sphere = m_scene.spheres.at(i);
            ImGui::PushID(i);
            if (ImGui::CollapsingHeader(std::string("Sphere").c_str(), ImGuiTreeNodeFlags_None))
            {
                ImGui::ColorEdit3("color", (float *)&sphere.color.x);
                ImGui::DragFloat3("position", (float *)&sphere.origin.x, 0.02, -20.0, 20);
                ImGui::DragFloat("radius", (float *)&sphere.radius, 0.01, 0.02, 2);
                ImGui::NewLine();
            }
            ImGui::PopID();
        }

        if (ImGui::Button("new sphere"))
        {
            m_scene.spheres.emplace_back();
        }

        Clear(m_clearColor);
        Render(currentViewportPanelSize);

        ImGui::End();
    }

    EditorLayer::RayTracerPayload EditorLayer::TraceRay(const Ray &ray)
    {
        int32_t closestSphereIndex = -1;
        float scalar = FLT_MAX;

        for (size_t index = 0; index < m_scene.spheres.size(); index++)
        {
            auto &&sphere = m_scene.spheres.at(index);
            float a = glm::dot(ray.direction, ray.direction);
            float b = 2 * (glm::dot(ray.origin, ray.direction) - glm::dot(ray.direction, sphere.origin));
            float c = glm::dot(ray.origin, ray.origin) + glm::dot(sphere.origin, sphere.origin) - 2 * glm::dot(ray.origin, sphere.origin) - sphere.radius * sphere.radius;

            float discriminant = b * b - 4.f * a * c;

            if (discriminant < 0.0f)
                continue;

            float t = ((-b - sqrt(discriminant)) / (2.f * a)); // using only one solution with the closer hit point

            if (t > 0.f && t < scalar)
            {
                scalar = t;
                closestSphereIndex = index;
            }
        }

        if (closestSphereIndex == -1)
        {
            return Miss();
        }

        return ClosestHit(ray, scalar, closestSphereIndex);
    }

    EditorLayer::RayTracerPayload EditorLayer::ClosestHit(const Ray &ray, float hitDistance, size_t sphereIndex)
    {
        Sphere *closestSphere = &m_scene.spheres.at(sphereIndex);
        RayTracerPayload payload;

        payload.hitPoint = ray.direction * hitDistance + ray.origin;                      // y = ax + b
        payload.surfaceNormal = glm::normalize(payload.hitPoint - closestSphere->origin); // perpendicular to a point on a surface
        payload.hitObjectIndex = sphereIndex;

        return payload;
    }

    EditorLayer::RayTracerPayload EditorLayer::Miss()
    {
        RayTracerPayload payload;
        payload.hitObjectIndex = -1;

        return payload;
    }

    glm::vec3 ScreenBlend(const glm::vec3 &a, const glm::vec3 &b)
    {
        return a + b - a * b;
    }

    glm::vec4 EditorLayer::PerPixel(size_t index)
    {
        Ray ray;
        ray.direction = m_camera.GetRayDirection(index);
        ray.origin = m_camera.GetPosition();

        glm::vec3 color(0.f);
        float multiplier = 1.f;
        size_t bounceLimit = 2;
        bool flag = false;
        for (size_t b = 0; b < bounceLimit; b++)
        {
            RayTracerPayload data = TraceRay(ray);

            if (data.hitObjectIndex == -1)
            {
                if (b == 0)
                    flag = true;
                break;
            }

            float d = glm::max(glm::dot(data.surfaceNormal, -glm::normalize(m_lightDirection)), 0.f); // cos(angle between vectors)

            Sphere *currentSphere = &m_scene.spheres.at(data.hitObjectIndex);
            glm::vec3 col = currentSphere->color;
            col *= d;
            color += col * multiplier;
            multiplier *= 0.7f;

            ray.direction = glm::reflect(ray.direction, data.surfaceNormal);
            ray.origin = data.hitPoint + data.surfaceNormal * 0.0001f;
        }
        if (flag)
            return glm::vec4(-1);

        return {color, 1.f};
    }

    void EditorLayer::Render(const ImVec2 &viewport)
    {
        for (size_t y = 0; y < viewport.y; y++)
        {
            for (size_t x = 0; x < viewport.x; x++)
            {
                size_t i = x + y * viewport.x;

                glm::vec4 pixel = PerPixel(i);

                if (pixel != glm::vec4(-1))
                    m_imageData[i] = Vec4ToPixel(pixel);
            }
        }
    }

    void EditorLayer::Clear(const glm::vec4 &color)
    {
        uint32_t val = Vec4ToPixel(color);

        for (size_t i = 0; i < c_imageDataSize; i++)
            m_imageData[i] = val;
    }

    uint32_t EditorLayer::Vec4ToPixel(const glm::vec4 &color)
    {
        glm::vec4 pixel = glm::clamp(color, 0.f, 1.f) * 255.f;
        return (uint8_t(pixel.a) << 24) | (uint8_t(pixel.b) << 16) | (uint8_t(pixel.g) << 8) | uint8_t(pixel.r);
    }

    void EditorLayer::OnDetach()
    {
        delete[] m_imageData;
    }

}