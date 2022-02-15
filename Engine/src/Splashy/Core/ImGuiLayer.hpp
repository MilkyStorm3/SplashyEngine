#pragma once
#include "Core/Layer.hpp"
#include "Input/Event.hpp"

namespace ant
{

    class ImGuiLayer
        : public Layer
    {
    public:
        ImGuiLayer() {}
        ~ImGuiLayer() {}

        virtual void OnAttach() override;
        virtual void OnUpdate() override;
        virtual void OnDraw() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event *event) override;

        void OnMouseMoved(Event *e);
        void OnMouseScrolled(Event *e);
        void OnMouseClicked(Event *e);
        void OnMouseReleased(Event *e);
        void OnKeyPressed(Event *e);
        void OnKeyReleased(Event *e);
        void OnWindowResized(Event *e);

        inline static void SetEventBlocking(bool block = true) { s_blockEvents = block; }
        inline static bool IsBlockingEvents() { return s_blockEvents; }

    private:
        static bool s_blockEvents;
    };

}