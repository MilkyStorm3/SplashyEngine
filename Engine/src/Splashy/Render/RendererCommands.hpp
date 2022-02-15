#pragma once
#include <glm/vec4.hpp>
namespace ant
{

    enum class RenderApi
    {
        None = 0,
        OpenGl
    };

    class RendererCommands
    {
    private:
        RendererCommands() {}
        ~RendererCommands() {}

    public:
        static bool InitGlfw();
        static bool InitGlew();
        static bool ShutdownGlfw();

        static void SetClearColor(const glm::vec4 &color);
        static void Clear();
        static void Clear(const glm::vec4 &color);

        static bool EnableGlDebugMessages();

        static RenderApi GetRenderApi();
    };

}