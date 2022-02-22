#pragma once
#include "Input/KeyCodes.hpp"
#include <glm/glm.hpp>
#include <Core/Core.hpp>

namespace ant
{
    class Input
    {
    public:
        Input() {}
        virtual ~Input() {}

        static void Init();
        static bool IsKeyPressed(KeyCode key);
        static bool IsMouseButtonPressed(MouseButtonCode buttonCode);
        static glm::vec2 MousePos();

    protected:
        virtual bool IsKeyPressed_IMPL(KeyCode key) = 0;
        virtual bool IsMouseButtonPressed_IMPL(MouseButtonCode buttonCode) = 0;
        virtual glm::vec2 MousePos_IMPL() = 0;

    private:
        static Scope<Input> s_instance;
    };

} // namespace ant
