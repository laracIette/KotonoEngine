#include "Mouse/Mouse.h"

#include <functional>
#include <GLFW/glfw3.h>
#include <Logging/log.h> 
#include <Window/Window.h>
#include <Containers/Map.h>

#define KT_LOG_IMPORTANCE_LEVEL_MOUSE ELogImportance::Low

static UMap<GLFWwindow*, std::function<void(EButton, i32)>> ButtonCallbacks{};
static UMap<GLFWwindow*, std::function<void(glm::vec2)>> CursorPositionCallbacks{};
static UMap<GLFWwindow*, std::function<void(glm::vec2)>> ScrollCallbacks{};

static void mousebutton_callback_(GLFWwindow* window, i32 button, i32 action, i32 mods)
{   
    auto const it{ ButtonCallbacks.Find(window) };
    if (ButtonCallbacks.IsValidIterator(it) && it->second)
    {
        it->second(static_cast<EButton>(button), action);
    }
}

static void cursorpos_callback_(GLFWwindow* window, f64 xpos, f64 ypos)
{
    auto const it{ CursorPositionCallbacks.Find(window) };
    if (CursorPositionCallbacks.IsValidIterator(it) && it->second)
    {
        it->second(glm::vec2{ xpos, ypos });
    }
}

static void scroll_callback_(GLFWwindow* window, f64 xoffset, f64 yoffset)
{
    auto const it{ ScrollCallbacks.Find(window) };
    if (ScrollCallbacks.IsValidIterator(it) && it->second)
    {
        it->second(glm::vec2{ xoffset, yoffset });
    }
}

UMouse::UMouse(UWindow& window)
    : window_{ window }
    , eventButton_{}
    , buttonStates_{}
{
}

void UMouse::Init()
{
    glfwSetMouseButtonCallback(window_.GetGLFWWindow(), mousebutton_callback_);
    glfwSetCursorPosCallback(window_.GetGLFWWindow(), cursorpos_callback_);
    glfwSetScrollCallback(window_.GetGLFWWindow(), scroll_callback_);
    
    ButtonCallbacks.TryEmplace(window_.GetGLFWWindow(), [this](EButton button, i32 action) { UpdateButton(button, action); });
    CursorPositionCallbacks.TryEmplace(window_.GetGLFWWindow(), [this](glm::vec2 const& position) { UpdateCursorPosition(position); });
    ScrollCallbacks.TryEmplace(window_.GetGLFWWindow(), [this](glm::vec2 const& delta) { UpdateScrollDelta(delta); });
}

void UMouse::Cleanup() const
{
    ButtonCallbacks.Remove(window_.GetGLFWWindow());
    CursorPositionCallbacks.Remove(window_.GetGLFWWindow());
    ScrollCallbacks.Remove(window_.GetGLFWWindow());
}

void UMouse::Update()
{
    for (size button{ 0 }; button < BUTTON_COUNT; ++button)
    {
        for (size inputState{ 0 }; inputState < INPUT_STATE_COUNT; ++inputState)
        {
            if (GetIsButtonState(button, inputState))
            {
                eventButton_.Broadcast(static_cast<EButton>(button), static_cast<EInputState>(inputState));
            }
        }

        if (GetIsButtonState(button, EInputState::Pressed))
        {
            SetIsButtonState(button, EInputState::Pressed, false);
        }
        else if (GetIsButtonState(button, EInputState::Released))
        {
            SetIsButtonState(button, EInputState::Released, false);
        }
    }

    if (cursorPosition_ != previousCursorPosition_)
    {
        eventMove_.Broadcast(GetCursorPositionDelta(), cursorPosition_);
        previousCursorPosition_ = cursorPosition_;
    }

    if (scrollDelta_ != glm::vec2{ 0.0f, 0.0f })
    {
        eventScroll_.Broadcast(scrollDelta_);
        scrollDelta_ = { 0.0f, 0.0f };
    }
}

auto UMouse::GetCursorPositionDelta() const -> glm::vec2
{
    return cursorPosition_ - previousCursorPosition_;
}

void UMouse::HideCursor() const
{
    glfwSetInputMode(window_.GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void UMouse::ShowCursor() const
{
    glfwSetInputMode(window_.GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void UMouse::UpdateButton(EButton button, i32 action)
{
    switch (action)
    {
    case GLFW_PRESS:
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_MOUSE, "Input", "GLFW_PRESS button {0}", std::to_underlying(button));

        SetIsButtonState(button, EInputState::Released, false);

        SetIsButtonState(button, EInputState::Pressed, true);
        SetIsButtonState(button, EInputState::Down, true);
        break;
    }
    case GLFW_RELEASE:
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_MOUSE, "Input", "GLFW_RELEASE button {0}", std::to_underlying(button));

        SetIsButtonState(button, EInputState::Pressed, false);
        SetIsButtonState(button, EInputState::Down, false);

        SetIsButtonState(button, EInputState::Released, true);
        break;
    }
    default:
        break;
    }
}

void UMouse::UpdateCursorPosition(glm::vec2 const& position)
{
    cursorPosition_ = position;
}

void UMouse::UpdateScrollDelta(glm::vec2 const& delta)
{
    scrollDelta_ += delta;
}
