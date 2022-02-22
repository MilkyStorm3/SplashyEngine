#include "Input/Input.hpp"
#include "Render/RendererCommands.hpp"
#include <Platform/Common/CommonInput.hpp>

namespace ant
{
    Scope<Input> Input::s_instance;
    
    void Input::Init()
    {
        if (s_instance)
            return;

        auto api = RendererCommands::GetRenderApi();

        if (api == RenderApi::OpenGl)
            s_instance = std::move(MakeScope<Common::CommonInput>());

        CORE_ASSERT(s_instance, "Picked RenderApi is not suported");
    }

    bool Input::IsKeyPressed(KeyCode key)
    {
        CORE_ASSERT(s_instance, "Input handler not initialized!");
        return s_instance->IsKeyPressed_IMPL(key);
    }

    bool Input::IsMouseButtonPressed(MouseButtonCode buttonCode)
    {
        CORE_ASSERT(s_instance, "Input handler not initialized!");
        return s_instance->IsMouseButtonPressed_IMPL(buttonCode);
    }

    glm::vec2 Input::MousePos()
    {
        CORE_ASSERT(s_instance, "Input handler not initialized!");
        return s_instance->MousePos_IMPL();
    }
} // namespace ant