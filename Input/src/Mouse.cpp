#include "Mouse.h"
#include <GLFW/glfw3.h>
#include <kotono_common/log.h> 
#include <kotono_platform/Window.h>

#define KT_LOG_IMPORTANCE_LEVEL_MOUSE ELogImportanceLevel::Low

void mousebutton_callback_(GLFWwindow* window, i32 button, i32 action, i32 mods);
void cursorpos_callback_(GLFWwindow* window, f64 xpos, f64 ypos);
void scroll_callback_(GLFWwindow* window, f64 xoffset, f64 yoffset);

void GMouse::Init(UWindow& window)
{
    glfwSetMouseButtonCallback(window.GetGLFWWindow(), mousebutton_callback_);
    glfwSetCursorPosCallback(window.GetGLFWWindow(), cursorpos_callback_);
    glfwSetScrollCallback(window.GetGLFWWindow(), scroll_callback_);
}

void GMouse::Update()
{
    for (size button{ 0 }; button < ButtonCount; ++button)
    {
        for (size inputState{ 0 }; inputState < InputStateCount; ++inputState)
        {
            if (buttonStates_[button][inputState])
            {
                buttonEvents_[button][inputState].Broadcast();
                eventAnyButton_.Broadcast(static_cast<EButton>(button), static_cast<EInputState>(inputState), cursorPosition_);
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

        if (scrollDelta_.x != 0.0f)
        {
            eventHorizontalScroll_.Broadcast(scrollDelta_.x);
        }
        if (scrollDelta_.y != 0.0f)
        {
            eventVerticalScroll_.Broadcast(scrollDelta_.y);
        }

        scrollDelta_ = { 0.0f, 0.0f };
    }
}

void GMouse::UpdateButton(EButton button, i32 action)
{
	const size buttonIndex{ to_index(button) };

    switch (action)
    {
    case GLFW_PRESS:
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_MOUSE, "Input", "GLFW_PRESS button {}", (u8)button);

        buttonStates_[buttonIndex][to_index(EInputState::Released)] = false;
        buttonStates_[buttonIndex][to_index(EInputState::Up)] = false;

        buttonStates_[buttonIndex][to_index(EInputState::Pressed)] = true;
        buttonStates_[buttonIndex][to_index(EInputState::Down)] = true;
        break;
    }
    case GLFW_RELEASE:
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_MOUSE, "Input", "GLFW_RELEASE button {}", (u8)button);

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

glm::vec2 GMouse::GetCursorPositionDelta() const
{
    return cursorPosition_ - previousCursorPosition_;
}

f32 GMouse::GetHorizontalScrollDelta() const
{
    return scrollDelta_.x;
}

f32 GMouse::GetVerticalScrollDelta() const
{
    return scrollDelta_.y;
}

UEvent<>& GMouse::GetEventButton(EButton button, EInputState inputState)
{
    return buttonEvents_[to_index(button)][to_index(inputState)];
}

bool GMouse::GetButtonState(EButton button, EInputState inputState) const
{
    return buttonStates_[to_index(button)][to_index(inputState)];
}

void mousebutton_callback_(GLFWwindow* window, i32 button, i32 action, i32 mods)
{
    Mouse.UpdateButton(static_cast<EButton>(button), action);
}

void cursorpos_callback_(GLFWwindow* window, f64 xpos, f64 ypos)
{
    Mouse.cursorPosition_ = { xpos, ypos };
}

void scroll_callback_(GLFWwindow* window, f64 xoffset, f64 yoffset)
{
    Mouse.scrollDelta_.x += static_cast<f32>(xoffset);
    Mouse.scrollDelta_.y += static_cast<f32>(yoffset);
}
