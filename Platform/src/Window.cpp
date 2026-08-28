#include "Window.h"

#include <GLFW/glfw3.h>
#include <kotono_common/log.h>
#include <stdexcept>

static UEvent<glm::uvec2> EventFramebufferSizeChanged{};

static void framebuffersize_callback_(GLFWwindow* window, i32 width, i32 height)
{
    // Replace to only freeze render
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    glm::uvec2 const size{ width, height };
    EventFramebufferSizeChanged.Broadcast(size);

    KT_LOG(ELogImportanceLevel::High, "Platform", "window resized: {} x {}", width, height);
}

void UWindow::Init()
{
    size_ = { 1600u, 900u };

    // Initialize GLFW
    if (!glfwInit())
    {
        throw std::runtime_error{ "Failed to initialize GLFW" };
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
    //framebuffersize_callback_(window_, size_.x, size_.y);

    EventFramebufferSizeChanged.AddListener(this, &UWindow::OnFramebufferSizeChanged);

    // Show the window after initialization
    glfwShowWindow(window_);
}

void UWindow::Cleanup()
{
    // Cleanup GLFW
    glfwDestroyWindow(window_);
    glfwTerminate();
}

bool UWindow::GetShouldClose(VkDevice device) const
{
    if (glfwWindowShouldClose(window_))
    {
        vkDeviceWaitIdle(device);
        return true;
    }

    glfwPollEvents();
    return false;
}

void UWindow::OnFramebufferSizeChanged(glm::uvec2 const& size)
{
    size_ = size;
    eventWindowResized_.Broadcast(size);
}
