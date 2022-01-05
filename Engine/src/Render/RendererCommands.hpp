#pragma once
#include <glm/vec4.hpp>
namespace ant
{

    class RendererCommands
    {
    private:
        RendererCommands() {}
        ~RendererCommands() {}

    public:
        static bool InitGlfw();
        static bool InitGlew();
        static bool ShutdownGlfw();

        static void SetClearColor(const glm::vec4& color);
        static void Clear();
        static void Clear(const glm::vec4& color);

        static bool EnableGlDebugMessages();

    };

}