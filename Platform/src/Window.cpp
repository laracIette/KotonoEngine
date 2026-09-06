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

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_ = glfwCreateWindow(size_.x, size_.y, "Kotono Engine", nullptr, nullptr);
    if (!window_)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetFramebufferSizeCallback(window_, framebuffersize_callback_);
    EventFramebufferSizeChanged.AddListener(this, &UWindow::OnFramebufferSizeChanged);

    glfwShowWindow(window_);
}

void UWindow::Cleanup()
{
    glfwDestroyWindow(window_);
}

bool UWindow::GetShouldClose() const
{
    if (glfwWindowShouldClose(window_))
    {
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
