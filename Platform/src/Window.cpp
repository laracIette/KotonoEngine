#include "Window.h"

#include <GLFW/glfw3.h>
#include <kotono_common/log.h>
#include <stdexcept>

static UEvent<GLFWwindow*, glm::uvec2> EventFramebufferSizeChanged{};

static void framebuffersize_callback_(GLFWwindow* window, i32 width, i32 height)
{
    // Replace to only freeze render
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    glm::uvec2 const size{ width, height };
    EventFramebufferSizeChanged.Broadcast(window, size);

    KT_LOG(ELogImportanceLevel::High, "Platform", "window resized: {0} x {0}", width, height);
}

void UWindow::Init(glm::uvec2 const& extent)
{
    extent_ = extent;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_ = glfwCreateWindow(extent.x, extent.y, "Kotono Engine", nullptr, nullptr);
    if (!window_)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetFramebufferSizeCallback(window_, framebuffersize_callback_);
    EventFramebufferSizeChanged.AddListener(this, &UWindow::OnFramebufferSizeChanged);

    glfwShowWindow(window_);
}

void UWindow::Cleanup() const
{
    glfwDestroyWindow(window_);
}

auto UWindow::GetShouldClose() const -> b8
{
    if (glfwWindowShouldClose(window_))
    {
        return true;
    }

    glfwPollEvents();
    return false;
}

void UWindow::OnFramebufferSizeChanged(GLFWwindow* window, glm::uvec2 const& size)
{
    if (window != window_)
    {
        return;
    }

    extent_ = size;
    eventWindowResized_.Broadcast(size);
}
