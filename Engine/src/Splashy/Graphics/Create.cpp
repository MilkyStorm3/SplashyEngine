#include "Pch.h"
#include "Render/RendererCommands.hpp"
#include "Graphics/FrameBuffer.hpp"
#include <Gl.h>

#include <Platform/OpenGl/OpenGlFrameBuffer.hpp>
#include <Platform/OpenGl/OpenGlVertexBuffer.hpp>
#include <Platform/OpenGl/OpenGlIndexBuffer.hpp>
#include <Platform/OpenGl/OpenGlVertexLayout.hpp>
#include <Platform/OpenGl/OpenGlShader.hpp>

namespace ant
{
    Ref<FrameBuffer> FrameBuffer::Create(const FramebufferSpecification& specs)
    {
        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl) return MakeRef<OpenGl::GlFrameBuffer>(specs);

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

    Ref<Shader> Shader::Create(const std::filesystem::path &filePath)
	{        
        auto api = RendererCommands::GetRenderApi();

		Ref<Shader> shader;

        if (api == RenderApi::OpenGl) shader = MakeRef<OpenGl::GlShader>();

        CORE_ASSERT(shader, "Shader object creation failed");
        
		shader->LoadFromFile(filePath);
		shader->Init();
		return shader;
	}

}