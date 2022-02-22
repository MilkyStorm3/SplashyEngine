#pragma once

#include "Input/KeyCodes.hpp"
#include "Graphics/Window.hpp"
#include "Camera/Camera.hpp"
#include <glm/glm.hpp>

namespace ant
{

    class Input
    {
    public:
        Input();
        ~Input();

        static bool IsKeyPressed(KeyCode key);
        static bool IsMouseButtonPressed(MouseButtonCode buttonCode);
        static void UseWindow(Ref<Window> window); //? set last active window here with event collector ???
        static glm::vec2 MousePos();

    private:
        static Ref<Window> s_activeWindow;
    };

} // namespace ant
