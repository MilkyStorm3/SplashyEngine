#pragma once

#include <Graphics/Window.hpp>
#include <Graphics/GraphicsContext.hpp>
class GLFWwindow;
namespace ant::Common
{

    class CommonWindow : public ant::Window
    {
    public:
        CommonWindow();
        virtual ~CommonWindow();

        virtual void Init(const std::string &title, uint32_t width, uint32_t height, bool resizeable) override;
        virtual void Update() override;

        virtual void SetEventCallback(const EventCallback &callback) override;
        virtual void SetVsync(bool vsync) override;
        virtual bool IsVsync() const override;
        virtual void *GetNativeWindow() const override;
        const EventCallback &GetEventCallback() const;

        virtual glm::ivec2 GetSize() const override;

    private:
        void SetCallbacks();

    private:
        GLFWwindow *m_window;
        bool m_vSync;
        Scope<GraphicsContext> m_context;
        const EventCallback *m_callback;
    };

}
