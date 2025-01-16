#include "Window.h"
#include "Timer.h"
#include "KotonoEngine.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

Window::Window() :
    _window(nullptr),
    _size(1600, 900)
{
}

Window::~Window()
{
    _vulkanInstance.Cleanup();

    // Cleanup GLFW
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Window::Init()
{
    InitGLFW();
    InitVulkan();

    // Show the window after initialization
    glfwShowWindow(_window);
}

GLFWwindow* Window::GetGLFWWindow() const
{
    return _window;
}

const bool Window::IsRunning() const
{
    if (glfwWindowShouldClose(_window))
    {
        return false;
    }

    glfwPollEvents();
    return true;
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(_window);
}

const glm::uvec2& Window::GetSize() const
{
    return _size;
}

void Window::SetSize(const glm::uvec2& size)
{
    _size = size;
}

void Window::InitGLFW()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        throw "Failed to initialize GLFW";
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create a windowed mode window and its OpenGL context
    _window = glfwCreateWindow(_size.x, _size.y, "Kotono Engine", nullptr, nullptr);
    if (!_window)
    {
        throw "Failed to create GLFW window";
    }

    // Make the window's context current
    glfwMakeContextCurrent(_window);

    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(_window, cursor_position_callback);
}

void Window::InitVulkan()
{
    _vulkanInstance.Init();
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //Engine->GetWindow()->SetSize(glm::uvec2(width, height));
    //Engine->GetFramebuffer()->ResizeTextures();

    std::cout << "Window resized: " << width << 'x' << height << std::endl;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    Engine->GetInputManager()->GetMouse().SetCursorPosition(glm::vec2(xpos, ypos));
    std::cout << "Mouse Position: (" << xpos << ", " << ypos << ")\n";
}