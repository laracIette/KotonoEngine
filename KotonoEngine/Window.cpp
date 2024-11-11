#include "Window.h"
#include "Timer.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Window::Window() :
    _window(nullptr),
    _windowSize(glm::uvec2(1600, 900)),
    _objectManager(nullptr)
{
}

Window::~Window()
{
    delete _objectManager;

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
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // Create a windowed mode window and its OpenGL context
    _window = glfwCreateWindow(_windowSize.x, _windowSize.y, "Kotono Engine", nullptr, nullptr);
    if (!_window)
    {
        throw "Failed to create GLFW window";
    }

    // Make the window's context current
    glfwMakeContextCurrent(_window);

    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

    // Initialize GLEW (for modern OpenGL functions)
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        throw "Failed to initialize GLEW";
    }

    // Show the window after initialization
    glfwShowWindow(_window);

    _objectManager = new ObjectManager();
    _objectManager->Init();
}

void Window::MainLoop()
{
    auto* renderTimer = new Timer();
    renderTimer->SetTargetDuration(1.0f / 60.0f);
    renderTimer->SetIsLoop(true);
    renderTimer->SetTimeout(
        [this]()
        {
            glClear(GL_COLOR_BUFFER_BIT);

            _objectManager->Draw();

            // Swap buffers
            glfwSwapBuffers(_window);
        }
    );
    renderTimer->Start();

    while (!glfwWindowShouldClose(_window))
    {
        _objectManager->Update();

        // Poll for and process events
        glfwPollEvents();
    }
}

ObjectManager* Window::GetObjectManager() const
{
    return _objectManager;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Adjust the viewport on window resize
    glViewport(0, 0, width, height);
    std::cout << "Window resized: " << width << 'x' << height << std::endl;
}