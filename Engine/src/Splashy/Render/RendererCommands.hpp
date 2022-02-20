#pragma once
#include <glm/vec4.hpp>
#include <memory>
namespace ant
{

    enum class RenderApi
    {
        None = 0,
        OpenGl,
        Vulcan
    };

    class RendererCommands
    {
    public:
        RendererCommands() {}
        virtual ~RendererCommands() {}

        // exposed functions
        static void Init();
        static void InitApiIfNeeded();
        static void Shutdown();
        static void SetClearColor(const glm::vec4 &color);
        static void Clear();
        static void Clear(const glm::vec4 &color);
        static RenderApi GetRenderApi();

    protected:
        virtual void Init_IMPL() = 0;
        virtual void InitApiIfNeeded_IMPL() = 0;
        virtual void Shutdown_IMPL() = 0;
        virtual void SetClearColor_IMPL(const glm::vec4 &color) = 0;
        virtual void Clear_IMPL() = 0;
        virtual void Clear_IMPL(const glm::vec4 &color) = 0;

    private:
        static std::unique_ptr<RendererCommands> s_instance;
    };

}