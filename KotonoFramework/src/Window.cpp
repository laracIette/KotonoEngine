#include "Window.h"
#include "Framework.h"
#include "Context.h"
#include <iostream>
#include "log.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

KtWindow::KtWindow() :
    _window(nullptr),
    _size(1600, 900)
{
}

void KtWindow::Init()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Create a windowed mode window and its OpenGL context
    _window = glfwCreateWindow(_size.x, _size.y, "Kotono Engine", nullptr, nullptr);
    if (!_window)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Make the window's context current
    glfwMakeContextCurrent(_window);

    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

    framebuffer_size_callback(_window, _size.x, _size.y);

    // Show the window after initialization
    glfwShowWindow(_window);
}

const bool KtWindow::GetShouldClose() const
{
    if (_shouldClose || glfwWindowShouldClose(_window))
    {
        vkDeviceWaitIdle(Framework.GetContext().GetDevice());
        return true;
    }

    glfwPollEvents();
    return false;
}

void KtWindow::SetShouldClose(const bool shouldClose)
{
    _shouldClose = shouldClose;
}

void KtWindow::Cleanup()
{
    // Cleanup GLFW
    glfwDestroyWindow(_window);
    glfwTerminate();
}

GLFWwindow* KtWindow::GetGLFWWindow() const
{
    return _window;
}

const glm::uvec2& KtWindow::GetSize() const
{
    return _size;
}

void KtWindow::SetSize(const glm::uvec2& size)
{
    _size = size;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{   
    // Replace to only freeze render
    while (width == 0 || height == 0)                                                   
    {                                                                                   
        glfwGetFramebufferSize(Framework.GetWindow().GetGLFWWindow(), &width, &height); 
        glfwWaitEvents();                                                               
    }                                                                                   

    WindowViewport.SetExtent(glm::uvec2(width, height));

    KT_DEBUG_LOG("window resized: %d x %d", width, height);
}