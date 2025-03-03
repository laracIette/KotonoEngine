#include "Window.h"
#include "Framework.h"
#include <iostream>
#include "log.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

KtWindow::KtWindow() :
    _window(nullptr),
    _size(1600, 900)
{
}

void KtWindow::Init()
{
    InitGLFW();
    InitVulkan();

    // Show the window after initialization
    glfwShowWindow(_window);
}

void KtWindow::MainLoop()
{
    while (!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
        _renderer.DrawFrame();
    }

    vkDeviceWaitIdle(_context.GetDevice());
}

void KtWindow::Cleanup()
{
    _renderer.Cleanup();
    _context.Cleanup();

    // Cleanup GLFW
    glfwDestroyWindow(_window);
    glfwTerminate();
}

GLFWwindow* KtWindow::GetGLFWWindow() const
{
    return _window;
}

KtContext& KtWindow::GetContext()
{
    return _context;
}

KtRenderer& KtWindow::GetRenderer()
{
    return _renderer;
}

void KtWindow::SwapBuffers() const
{
    glfwSwapBuffers(_window);
}

const glm::uvec2& KtWindow::GetSize() const
{
    return _size;
}

void KtWindow::SetSize(const glm::uvec2& size)
{
    _size = size;
}

void KtWindow::InitGLFW()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        throw "Failed to initialize GLFW";
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

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

void KtWindow::InitVulkan()
{
    _context.Init();
    _renderer.Init();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //Framework.GetWindow().SetSize(glm::uvec2(width, height));
    //Framework.GetFramebuffer().ResizeTextures();

    Framework.GetWindow().GetRenderer().OnFramebufferResized();
    std::cout << "Window resized: " << width << 'x' << height << std::endl;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    Framework.GetInputManager().GetMouse().SetCursorPosition(glm::vec2(xpos, ypos));
}