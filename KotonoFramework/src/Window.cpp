#include "Window.h"
#include "Framework.h"
#include "Context.h"
#include "Viewport.h"
#include "log.h"

void framebuffersize_callback_(GLFWwindow* window, int width, int height);

KtWindow::KtWindow() :
    window_(nullptr),
    size_(1600, 900),
    shouldClose_(false)
{
}

void KtWindow::Init()
{
    WindowViewport.SetAspectRatio(static_cast<float>(size_.x) / size_.y);
    WindowViewport.SetIsKeepAspectRatio(true);

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

const bool KtWindow::GetShouldClose() const
{
    if (shouldClose_ || glfwWindowShouldClose(window_))
    {
        vkDeviceWaitIdle(Framework.GetContext().GetDevice());
        return true;
    }

    glfwPollEvents();
    return false;
}

void KtWindow::SetShouldClose(const bool shouldClose)
{
    shouldClose_ = shouldClose;
}

void KtWindow::Cleanup()
{
    // Cleanup GLFW
    glfwDestroyWindow(window_);
    glfwTerminate();
}

GLFWwindow* KtWindow::GetGLFWWindow() const
{
    return window_;
}

const glm::uvec2& KtWindow::GetSize() const
{
    return size_;
}

void KtWindow::SetSize(const glm::uvec2& size)
{
    size_ = size;
}

void framebuffersize_callback_(GLFWwindow* window, int width, int height)
{   
    // Replace to only freeze render
    while (width == 0 || height == 0)                                                   
    {                                                                                   
        glfwGetFramebufferSize(Framework.GetWindow().GetGLFWWindow(), &width, &height); 
        glfwWaitEvents();                                                               
    } 

    WindowViewport.SetExtent(glm::uvec2(width, height));

    KT_DEBUG_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "window resized: %d x %d", width, height);
}