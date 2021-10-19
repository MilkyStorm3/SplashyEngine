#pragma once

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

        static bool EnableGlDebugMessages();

    };

}