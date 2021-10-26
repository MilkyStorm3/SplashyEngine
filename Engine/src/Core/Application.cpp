#include "Application.hpp"
#include "Logger.hpp"
#include <Gl.h>
#include "Core/Core.hpp"
#include "Render/RendererCommands.hpp"
#include "Input/Event.hpp"
#include "debug/ImGuiLayer.hpp"

//! TEMP
#include <Gl.h>
// #include <Graphics/VertexBuffer.hpp>
// #include <Graphics/IndexBuffer.hpp>
#include <Graphics/o_Buffer.hpp>
#include <Graphics/Shader.hpp>
#include <filesystem>

void test();
namespace ant
{
    Application *Application::s_instance;

    Application::~Application()
    {
        //todo shutdown glew
    }

    void Application::Init()
    {
        ant::Logger::Init();

        CORE_INFO("Hello!");

        m_window.SetEventCallback(CORE_BIND_EVENT_FN(this, Application::OnEvent));

        m_window.Init({m_appdata.windowSettings.width,
                       m_appdata.windowSettings.height,
                       m_appdata.windowSettings.title,
                       true, false});

        RendererCommands::EnableGlDebugMessages();

        // m_layerStack.PushOverlay(MakeRef<ImGuiLayer>());

        // RendererCommands::SetClearColor({1.f,0.f,1.f,1.f});
        RendererCommands::SetClearColor({0.3f, 0.3f, 0.3f, 1.f});

        m_appInitFn();
    }

    void Application::Run()
    {

        //todo load a shader and comfigure a filesystem root path

        // auto shpath = Shader::EngineShaderDir().append("triangleShader.glsl");

        // Shader shader = Shader(shpath.string());
        // Shader shader = Shader("triangleShader.glsl");
        // shader.CreateShader();

        // CORE_INFO(shpath.string().c_str());

        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f};

        unsigned int VBO;
        glGenBuffers(1, &VBO);

        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // shader.BindShader();

        while (m_appdata.running)
        {
            // m_layerStack.OnUpdate();
            RendererCommands::Clear();

            // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            //! test Ib and Vb here

            // m_layerStack.OnDraw();
            m_window.Update();
        }
    }

    void Application::OnEvent(Event &e)
    {
        CORE_INFO("Event triggered {0}", e.GetStringLog());

        m_layerStack.OnEvent(&e);

        EventDispatcher d(e);

        d.DispatchEvent<WindowClosedEvent>(
            [this](WindowClosedEvent &wce) -> bool
            {
                m_appdata.running = false;
                return true;
            });
    }

}