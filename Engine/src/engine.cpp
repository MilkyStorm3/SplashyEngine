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

void test()
{
    // auto j3 = nlohmann::json::parse(R"({"happy": true, "pi": 3.141})");

    // glm::ivec2 vec = {5, 2};
    // std::cout << "x: " << vec.x << "y: " << vec.y << "\n\n";

    // std::cout<<j3["happy"]<< j3["pi"]<<"\n\n";

    ant::Window window;

    window.SetEventCallback([](ant::Event &e) {

    });

       window.Init({800,
                 600,
                 "title",
                 true, false});

    ant::Shader shader;
    shader.LoadFromFile("shaders/triangleShader.glsl");
    shader.Init();

    // shader.Init(vertexSource, fragmentSource);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0,
        0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0,
        -0.5f, 0.5f, 0.0f, 0.0, 0.0, 1.0,
        0.5f, 0.5f, 0.0f, 1.0, 1.0, 1.0
    };

    ant::VertexBuffer vb;
    vb.GetLayout().PushAttribute(ant::AttributeType::vec3f);
    vb.GetLayout().PushAttribute(ant::AttributeType::vec3f);
    vb.UploadData(&vertices[0], sizeof(vertices));

    vb.Bind();

    shader.Bind();

    ant::IndexBuffer ib; //! checkout IndexBuffer STH DOSENT WORK with it
    uint32_t indicies[] ={
        0,1,2,2,1,3
    };
    ib.Bind();
    ib.UploadData(&indicies[0], sizeof(ib));
    ib.Bind();


    while (!glfwWindowShouldClose(window.GetNativeWindow()))
    {
        // float ratio;
        int width, height;
        glfwGetFramebufferSize(window.GetNativeWindow(), &width, &height);
        // // ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glLoadIdentity();
        // glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        // glBegin(GL_TRIANGLES);
        // glColor3f(1.f, 0.f, 0.f);
        // glVertex3f(-0.6f, -0.4f, 0.f);
        // glColor3f(0.f, 1.f, 0.f);
        // glVertex3f(0.6f, -0.4f, 0.f);
        // glColor3f(0.f, 0.f, 1.f);
        // glVertex3f(0.f, 0.6f, 0.f);

        // glEnd();

        window.Update();
    }
    ant::RendererCommands::ShutdownGlfw();
    exit(EXIT_SUCCESS);
}