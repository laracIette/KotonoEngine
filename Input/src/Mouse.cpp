#include "Mouse.h"
#include <GLFW/glfw3.h>
#include <kotono_common/log.h> 
#include <kotono_platform/Window.h>

#define KT_LOG_IMPORTANCE_LEVEL_MOUSE ELogImportanceLevel::Low

static UEvent<GLFWwindow*, EButton, i32> EventMouseButton{};
static UEvent<GLFWwindow*, glm::vec2> EventCursorPosition{};
static UEvent<GLFWwindow*, glm::vec2> EventScroll{};

static void mousebutton_callback_(GLFWwindow* window, i32 button, i32 action, i32 mods)
{
    EventMouseButton.Broadcast(window, static_cast<EButton>(button), action);
}

static void cursorpos_callback_(GLFWwindow* window, f64 xpos, f64 ypos)
{
    EventCursorPosition.Broadcast(window, glm::vec2{ xpos, ypos });
}

static void scroll_callback_(GLFWwindow* window, f64 xoffset, f64 yoffset)
{
    EventScroll.Broadcast(window, glm::vec2{ xoffset, yoffset });
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

    EventMouseButton.AddListener(this, &UMouse::UpdateButton);
    EventCursorPosition.AddListener(this, &UMouse::UpdateCursorPosition);
    EventScroll.AddListener(this, &UMouse::UpdateScrollDelta);
}

void UMouse::Cleanup()
{
    EventMouseButton.RemoveListener(this, &UMouse::UpdateButton);
    EventCursorPosition.RemoveListener(this, &UMouse::UpdateCursorPosition);
    EventScroll.RemoveListener(this, &UMouse::UpdateScrollDelta);
}

void UMouse::Update()
{
    for (size button{ 0 }; button < ButtonCount; ++button)
    {
        for (size inputState{ 0 }; inputState < InputStateCount; ++inputState)
        {
            if (buttonStates_[button][inputState])
            {
                eventButton_.Broadcast(static_cast<EButton>(button), static_cast<EInputState>(inputState), cursorPosition_);
            }
        }

        if (buttonStates_[button][to_index(EInputState::Pressed)])
        {
            buttonStates_[button][to_index(EInputState::Pressed)] = false;
        }
        else if (buttonStates_[button][to_index(EInputState::Released)])
        {
            buttonStates_[button][to_index(EInputState::Released)] = false;
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

bool UMouse::GetButtonState(EButton button, EInputState inputState) const
{
    return buttonStates_[to_index(button)][to_index(inputState)];
}

void UMouse::HideCursor() const
{
    glfwSetInputMode(window_.GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void UMouse::ShowCursor() const
{
    glfwSetInputMode(window_.GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void UMouse::UpdateButton(GLFWwindow* window, EButton button, i32 action)
{
    if (window != window_.GetGLFWWindow())
    {
        return;
    }

    const size buttonIndex{ to_index(button) };

    switch (action)
    {
    case GLFW_PRESS:
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_MOUSE, "Input", "GLFW_PRESS button {0}", (u8)button);

        buttonStates_[buttonIndex][to_index(EInputState::Released)] = false;
        buttonStates_[buttonIndex][to_index(EInputState::Up)] = false;

        buttonStates_[buttonIndex][to_index(EInputState::Pressed)] = true;
        buttonStates_[buttonIndex][to_index(EInputState::Down)] = true;
        break;
    }
    case GLFW_RELEASE:
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_MOUSE, "Input", "GLFW_RELEASE button {0}", (u8)button);

        buttonStates_[buttonIndex][to_index(EInputState::Pressed)] = false;
        buttonStates_[buttonIndex][to_index(EInputState::Down)] = false;

        buttonStates_[buttonIndex][to_index(EInputState::Released)] = true;
        buttonStates_[buttonIndex][to_index(EInputState::Up)] = true;
        break;
    }
    default:
        break;
    }
}

void UMouse::UpdateCursorPosition(GLFWwindow* window, glm::vec2 const& position)
{
    if (window != window_.GetGLFWWindow())
    {
        return;
    }

    cursorPosition_ = position;
}

void UMouse::UpdateScrollDelta(GLFWwindow* window, glm::vec2 const& delta)
{
    if (window != window_.GetGLFWWindow())
    {
        return;
    }

    scrollDelta_ += delta;
}
