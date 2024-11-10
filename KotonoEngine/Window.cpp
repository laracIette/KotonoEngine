#include "Window.h"
#include "ObjectManager.h"
#include "Timer.h"
#include <iostream>

Window::Window() :
    _window(nullptr)
{
}

Window::~Window()
{
    // Cleanup GLFW
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Window::Init()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        throw "Failed to initialize GLFW";
    }

    // Set OpenGL version (e.g., 3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    _window = glfwCreateWindow(1600, 900, "OpenGL Window", nullptr, nullptr);
    if (!_window)
    {
        throw "Failed to create GLFW window";
    }

    // Make the window's context current
    glfwMakeContextCurrent(_window);

    // Initialize GLEW (for modern OpenGL functions)
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        throw "Failed to initialize GLEW";
    }
}

void Window::MainLoop()
{
    ObjectManager* objectManager = new ObjectManager();

    Timer* renderTimer = new Timer();
    renderTimer->SetTargetDuration(1.0f / 60.0f);
    renderTimer->SetIsLoop(true);
    renderTimer->SetTimeout(
        [this, objectManager]()
        {
            glClear(GL_COLOR_BUFFER_BIT);

            objectManager->Draw();

            // Swap buffers
            glfwSwapBuffers(this->_window);
        }
    );
    renderTimer->Start();

    objectManager->Create(renderTimer);

    while (!glfwWindowShouldClose(_window))
    {
        objectManager->Update();

        // Poll for and process events
        glfwPollEvents();
    }

    delete objectManager;
}
