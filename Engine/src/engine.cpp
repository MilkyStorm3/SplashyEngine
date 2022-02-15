#include <iostream>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include "Gl.h"

#include <Graphics/Window.hpp>
#include <Render/RendererCommands.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/VertexBuffer.hpp>
#include <Graphics/IndexBuffer.hpp>
#include <Core/Logger.hpp>

#include <fstream>

#ifdef TEST_FN

void test()
{
    // auto j3 = nlohmann::json::parse(R"({"happy": true, "pi": 3.141})");

    // glm::ivec2 vec = {5, 2};
    // std::cout << "x: " << vec.x << "y: " << vec.y << "\n\n";

    // std::cout<<j3["happy"]<< j3["pi"]<<"\n\n";

    ant::Window window;

    window.SetEventCallback([](ant::Event &e) {

    });

    window.Init({800, 600, "title", true, false});

    ant::RendererCommands::EnableGlDebugMessages();
    ant::RendererCommands::SetClearColor({0.2, 0.2, 0.2, 1.f});

    ant::Shader shader;
    shader.LoadFromFile("assets/shaders/triangleShader.glsl");
    shader.Init();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0,
        0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0,
        -0.5f, 0.5f, 0.0f, 0.0, 0.0, 1.0,
        0.5f, 0.5f, 0.0f, 1.0, 1.0, 1.0};

    ant::VertexBuffer vb;
    vb.GetLayout().PushAttribute(ant::AttributeType::vec3f);
    vb.GetLayout().PushAttribute(ant::AttributeType::vec3f);
    vb.UploadData(&vertices[0], sizeof(vertices));

    vb.Bind();

    shader.Bind();

    ant::IndexBuffer ib;

    uint32_t indices[] = {
        0, 1, 2, 2, 1, 3};

    ib.UploadData(&indices[0], sizeof(indices));
    ib.Bind();

    while (!glfwWindowShouldClose(window.GetNativeWindow()))
    {
        int width, height;
        glfwGetFramebufferSize(window.GetNativeWindow(), &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(
            GL_TRIANGLES,
            ib.GetCount(),
            GL_UNSIGNED_INT,
            (void *)0);

        window.Update();
    }
    ant::RendererCommands::ShutdownGlfw();
    exit(EXIT_SUCCESS);
}

#endif