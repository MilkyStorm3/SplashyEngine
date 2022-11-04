#include "Pch.h"
#include "Render/RendererCommands.hpp"

#include <Platform/OpenGl/OpenGlFrameBuffer.hpp>
#include <Platform/OpenGl/OpenGlVertexBuffer.hpp>
#include <Platform/OpenGl/OpenGlIndexBuffer.hpp>
#include <Platform/OpenGl/OpenGlVertexLayout.hpp>
#include <Platform/OpenGl/OpenGlShader.hpp>
#include <Platform/OpenGl/OpenGlUniformBuffer.hpp>
#include <Platform/OpenGl/OpenGlGraphicsContext.hpp>
#include <Platform/OpenGl/OpenGlTexture2D.hpp>
#include <Platform/OpenGl/OpenGlVertexArray.hpp>

#include <Platform/Common/CommonWindow.hpp>

namespace ant
{
    Ref<FrameBuffer> FrameBuffer::Create(const FramebufferSpecification &specs)
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl)
            return MakeRef<OpenGl::GlFrameBuffer>(specs);

        CORE_ASSERT(false, "Picked RenderApi is not suported");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create()
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl)
            return MakeRef<OpenGl::GlVertexBuffer>();

        CORE_ASSERT(false, "Picked RenderApi is not suported");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create()
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl)
            return MakeRef<OpenGl::GlIndexBuffer>();

        CORE_ASSERT(false, "Picked RenderApi is not suported");
        return nullptr;
    }

    Ref<VertexLayout> VertexLayout::Create()
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl)
            return MakeRef<OpenGl::GlVertexLayout>();

        CORE_ASSERT(false, "Picked RenderApi is not suported");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::filesystem::path &filePath, bool trackSource, bool useTextShaders)
    {
        auto api = RendererCommands::GetRenderApi();

        Ref<Shader> shader;

        if (api == RenderApi::OpenGl)
            shader = MakeRef<OpenGl::GlShader>(trackSource, useTextShaders);

        CORE_ASSERT(shader, "Shader object creation failed");

        shader->LoadFromFile(filePath);
        shader->Init();
        return shader;
    }

    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl)
            return MakeRef<OpenGl::GlUniformBuffer>(size, binding);

        CORE_ASSERT(false, "Picked RenderApi is not suported");
        return nullptr;
    }

    Scope<GraphicsContext> GraphicsContext::Create(void *nativeWindow)
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl)
            return MakeScope<OpenGl::GlGraphicsContext>(nativeWindow);

        CORE_ASSERT(false, "Picked RenderApi is not suported");
        return nullptr;
    }

    Ref<Window> Window::Create() // todo make it os dependent instead of api
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl)
            return MakeRef<Common::CommonWindow>();

        CORE_ASSERT(false, "Picked RenderApi is not suported");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::filesystem::path &path)
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl)
            return MakeRef<OpenGl::GlTexture2D>(path);

        CORE_ASSERT(false, "Picked RenderApi is not suported");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl)
            return MakeRef<OpenGl::GlTexture2D>(width, height);

        CORE_ASSERT(false, "Picked RenderApi is not suported");
        return nullptr;
    }

    Ref<VertexArray> VertexArray::Create()
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl)
            return MakeRef<OpenGl::GlVertexArray>();

        CORE_ASSERT(false, "Picked RenderApi is not suported");
        return nullptr;
    }

}