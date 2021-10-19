#pragma once
#include "Input/KeyCodes.hpp"
#include "Graphics/Window.hpp"
#include "Camera/Camera.hpp"
#include <glm/glm.hpp>
namespace df
{

    class Input
    {
    public:
        Input();
        ~Input();

        static bool IsKeyPressed(KeyCode key);
        static bool IsMouseButtonPressed(MouseButtonCode buttonCode);
        static void UseWindow(Window *window); //? set last active window here with event collector ???
        static glm::vec2 MousePos();
        static glm::vec2 MouseWorldPos(const OrthographicCamera &camera);

    private:
        static Window *s_activeWindow;
    };

} // namespace df