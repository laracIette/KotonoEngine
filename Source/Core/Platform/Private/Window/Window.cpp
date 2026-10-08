#include "Window/Window.h"

#include <check.h>
#include <functional>
#include <GLFW/glfw3.h>
#include <glm/vector_relational.hpp>
#include <Logging/log.h>
#include <stdexcept>
#include <Containers/Map.h>

static UMap<GLFWwindow*, std::function<void(glm::uvec2)>> FramebufferSizeChangedCallbacks{};

static void framebuffersize_callback_(GLFWwindow* window, i32 width, i32 height)
{        
    auto const it{ FramebufferSizeChangedCallbacks.Find(window) };
    if (FramebufferSizeChangedCallbacks.IsValidIterator(it) && it->second)
    {
        it->second(glm::uvec2{ width, height });
    }

    KT_LOG(ELogImportanceLevel::High, "Platform", "window resized: {0} x {0}", width, height);
}

void UWindow::Init(glm::uvec2 const& extent, std::string_view name)
{
    Check(Abort, !glm::any(glm::equal(extent, glm::uvec2{ 0, 0 })), "requested window area is 0!");

    extent_ = extent; 
    isMinimized_ = false;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_ = glfwCreateWindow(extent.x, extent.y, name.data(), nullptr, nullptr);
	Check(Abort, window_, "failed to create GLFW window!");

    glfwSetFramebufferSizeCallback(window_, framebuffersize_callback_);
    FramebufferSizeChangedCallbacks.TryEmplace(window_, [this](glm::uvec2 const& size) { OnFramebufferSizeChanged(size); });

    glfwShowWindow(window_);
}

void UWindow::Cleanup() const
{
    FramebufferSizeChangedCallbacks.Remove(window_);

    glfwDestroyWindow(window_);
}

auto UWindow::GetShouldClose() const -> b8
{
    return glfwWindowShouldClose(window_);
}

void UWindow::OnFramebufferSizeChanged(glm::uvec2 const& size)
{
    if (glm::any(glm::equal(size, glm::uvec2{ 0, 0 })))
    {
        isMinimized_ = true;
        return;
    }

    isMinimized_ = false;
    extent_ = size;
    eventWindowResized_.Broadcast(size);
}
