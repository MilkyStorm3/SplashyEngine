#include "Pch.h"
#include "Render/RendererCommands.hpp"
#include "Graphics/FrameBuffer.hpp"
#include <Gl.h>

#include <Platform/OpenGl/OpenGlFrameBuffer.hpp>
#include <Platform/OpenGl/OpenGlVertexBuffer.hpp>
#include <Platform/OpenGl/OpenGlIndexBuffer.hpp>
#include <Platform/OpenGl/OpenGlVertexLayout.hpp>

namespace ant
{
    Ref<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height)
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl) return MakeRef<OpenGl::GlFrameBuffer>(width, height);

        CORE_ASSERT(false, "Picked RenderApi is not suported");
    }

    Ref<VertexBuffer> VertexBuffer::Create()
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl) return MakeRef<OpenGl::GlVertexBuffer>();

        CORE_ASSERT(false, "Picked RenderApi is not suported");
    }

    Ref<IndexBuffer> IndexBuffer::Create()
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl) return MakeRef<OpenGl::GlIndexBuffer>();

        CORE_ASSERT(false, "Picked RenderApi is not suported");
    }

    Ref<VertexLayout> VertexLayout::Create()
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl) return MakeRef<OpenGl::GlVertexLayout>();

        CORE_ASSERT(false, "Picked RenderApi is not suported");
    }

}