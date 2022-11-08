#pragma once
#include "Input/KeyCodes.hpp"
#include <glm/glm.hpp>
#include <Core/Core.hpp>

namespace ant
{
    enum class CursorStyle
    {
        Disabled = 0,
        Hidden,
        Normal
    };

    class Input
    {
    public:
        Input() {}
        virtual ~Input() {}

        static void Init();
        static bool IsKeyPressed(KeyCode key);
        static bool IsMouseButtonPressed(MouseButtonCode buttonCode);
        static glm::vec2 MousePos();
        static void SetMousePos(const glm::vec2 &pos);
        static void SetCursor(CursorStyle cursor);

    protected:
        virtual bool IsKeyPressed_IMPL(KeyCode key) = 0;
        virtual bool IsMouseButtonPressed_IMPL(MouseButtonCode buttonCode) = 0;
        virtual glm::vec2 MousePos_IMPL() = 0;
        virtual void SetCursor_IMPL(CursorStyle cursor) = 0;
        virtual void SetMousePos_IMPL(const glm::vec2 &pos) = 0;

    private:
        static Scope<Input> s_instance;
    };

} // namespace ant
