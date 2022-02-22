#pragma once
#include <Input/Input.hpp>

namespace ant::Common
{

    class CommonInput : public ant::Input
    {
    public:
        CommonInput() {}
        ~CommonInput() {}

        virtual bool IsKeyPressed_IMPL(KeyCode key) override;
        virtual bool IsMouseButtonPressed_IMPL(MouseButtonCode buttonCode) override;
        virtual glm::vec2 MousePos_IMPL() override;

        static void* s_activeNativeWindow;
    };


    
} // namespace ant::Common
