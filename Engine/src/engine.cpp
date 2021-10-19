#include <iostream>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include "Gl.h"

void test()
{
    auto j3 = nlohmann::json::parse(R"({"happy": true, "pi": 3.141})");

    glm::ivec2 vec = {5, 2};
    std::cout << "x: " << vec.x << "y: " << vec.y << "\n\n";

    std::cout<<j3["happy"]<< j3["pi"]<<"\n\n";

    GLFWwindow *window;
    if (!glfwInit())
        exit(EXIT_FAILURE);
        
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}