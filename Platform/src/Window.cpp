#include "Window.h"
#include "Context.h"
#include "WindowViewport.h"
#include <kotono_common/log.h>

void framebuffersize_callback_(GLFWwindow* window, int width, int height);

void GWindow::Init()
{
    size_ = { 1600, 900 };

    SWindowViewport::SetIsKeepAspectRatio(false);
    SWindowViewport::SetAspectRatio(static_cast<float>(size_.x) / size_.y);

    // Initialize GLFW
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Create a windowed mode window and its OpenGL context
    window_ = glfwCreateWindow(size_.x, size_.y, "Kotono Engine", nullptr, nullptr);
    if (!window_)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Make the window's context current
    glfwMakeContextCurrent(window_);

    glfwSetFramebufferSizeCallback(window_, framebuffersize_callback_);

    framebuffersize_callback_(window_, size_.x, size_.y);

    // Show the window after initialization
    glfwShowWindow(window_);
}

void GWindow::Cleanup()
{
    // Cleanup GLFW
    glfwDestroyWindow(window_);
    glfwTerminate();
}

bool GWindow::GetShouldClose() const
{
    if (shouldClose_ || glfwWindowShouldClose(window_))
    {
        vkDeviceWaitIdle(Context.GetDevice());
        return true;
    }

    glfwPollEvents();
    return false;
}

void GWindow::SetShouldClose(const bool shouldClose)
{
    shouldClose_ = shouldClose;
}

GLFWwindow* GWindow::GetGLFWWindow() const
{
    return window_;
}

const glm::uvec2& GWindow::GetSize() const
{
    return size_;
}

UEvent<>& GWindow::GetEventWindowResized()
{
    return eventWindowResized_;
}

void GWindow::SetSize(const glm::uvec2& size)
{
    size_ = size;
}

void framebuffersize_callback_(GLFWwindow* window, int width, int height)
{   
    // Replace to only freeze render
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    SWindowViewport::SetExtent({ width, height });

    Window.GetEventWindowResized().Broadcast();

    KT_LOG(ELogImportanceLevel::High, "Platform", "window resized: {} x {}", width, height);
}