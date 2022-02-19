#include "Core/Application.hpp"
#include "RendererCommands.hpp"
#include <Gl.h>
#include "Core/Core.hpp"
#include <stb_image.h>
#include "debug/Instrumentation.hpp"
#include "Platform/OpenGl/OpenGlRendererCommands.hpp"

namespace ant
{
    std::unique_ptr<RendererCommands> RendererCommands::s_instance;

    RenderApi RendererCommands::GetRenderApi()
    {
        return Application::GetInstance()->GetRenderApi();
    }

    void RendererCommands::Init()
    {
        if (s_instance)
        {
            s_instance->Init_IMPL();
            return;
        }

        auto api = GetRenderApi();

        if (api == RenderApi::OpenGl)
            s_instance = std::move(std::make_unique<OpenGl::GlRendererCommands>());

        CORE_ASSERT(s_instance, "Picked RenderApi is not suported");
        s_instance->Init_IMPL();
    }

    void RendererCommands::InitApiIfNeeded()
    {
        CORE_ASSERT(s_instance, "Renderer commands not initialized");
        s_instance->InitApiIfNeeded_IMPL();
    }

    void RendererCommands::Shutdown()
    {
        if (s_instance)
            s_instance->Shutdown_IMPL();
    }

    void RendererCommands::SetClearColor(const glm::vec4 &color)
    {
        CORE_ASSERT(s_instance, "Renderer commands not initialized");
        s_instance->SetClearColor_IMPL(color);
    }

    void RendererCommands::Clear()
    {
        CORE_ASSERT(s_instance, "Renderer commands not initialized");
        s_instance->Clear_IMPL();
    }

    void RendererCommands::Clear(const glm::vec4 &color)
    {
        CORE_ASSERT(s_instance, "Renderer commands not initialized");
        s_instance->Clear_IMPL(color);
    }
}