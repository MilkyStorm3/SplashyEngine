#include <iostream>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include "Gl.h"

#include <Graphics/Window.hpp>
#include <Render/RendererCommands.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/VertexBuffer.hpp>
#include <Graphics/IndexBuffer.hpp>

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

    // auto shpath = ant::Shader::EngineShaderDir().append("triangleShader.glsl");

    // ant::Shader shader = ant::Shader(shpath.string());
    // shader.CreateShader();

    // CORE_INFO(shpath.string().c_str());

    std::string vertexShaderSource = R"(
#version 450 core
		
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;


void main()
{
 	gl_Position = vec4(position.x, position.y, position.z, 1.0);
}

        )";

    std::string fragmentShaderSource = R"(
#version 450 core
out vec4 color;

void main(){

	color = vec4(0.8, 0.2,0.2,1.0);
}
        )";

    ant::Shader shader;
    shader.Init(vertexShaderSource, fragmentShaderSource);
    // shader.LoadFromFile(shpath.string());
    // shader.Init();

    // std::cout << std::filesystem::current_path().parent_path().append("Engine/shader").string();
    std::cout<<"stest\n";
    // std::cout << std::filesystem::current_path().string();
    auto a = std::filesystem::current_path();
    // std::cout<<shader.GetSource().vertex;
    // std::cout<<shader.GetSource().fragment;
    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0,
        0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0,
        0.0f, 0.5f, 0.0f, 0.0, 0.0, 1.0

    };

    ant::VertexBuffer vb;
    vb.GetLayout().PushAttribute(ant::AttributeType::vec3f);
    vb.GetLayout().PushAttribute(ant::AttributeType::vec3f);
    vb.UploadData(&vertices[0], sizeof(vertices));

    vb.Bind();

    shader.Bind();

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