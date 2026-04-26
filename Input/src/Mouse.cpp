#include "Mouse.h"
#include <GLFW/glfw3.h>
#include <kotono_common/log.h> 
#include <kotono_platform/Window.h>

#define KT_LOG_IMPORTANCE_LEVEL_MOUSE ELogImportanceLevel::Low

void mousebutton_callback_(GLFWwindow* window, int button, int action, int mods);
void cursorpos_callback_(GLFWwindow* window, double xpos, double ypos);
void scroll_callback_(GLFWwindow* window, double xoffset, double yoffset);

void SMouse::Init()
{
    glfwSetMouseButtonCallback(Window.GetGLFWWindow(), mousebutton_callback_);
    glfwSetCursorPosCallback(Window.GetGLFWWindow(), cursorpos_callback_);
    glfwSetScrollCallback(Window.GetGLFWWindow(), scroll_callback_);
}

void SMouse::Update()
{
    for (size button{ 0 }; button < ButtonCount; ++button)
    {
        for (size inputState{ 0 }; inputState < InputStateCount; ++inputState)
        {
            if (buttonStates_[button][inputState])
            {
                buttonEvents_[button][inputState].Broadcast();
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
        eventMove_.Broadcast(CursorPositionDelta());
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

void SMouse::UpdateButton(const EButton button, const int action)
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

const glm::vec2& SMouse::PreviousCursorPosition() const
{
    return previousCursorPosition_;
}

const glm::vec2& SMouse::CursorPosition() const
{
    return cursorPosition_;
}

glm::vec2 SMouse::CursorPositionNormalized() const
{
    const auto& windowSize = Window.GetSize();
    return 2.0f * cursorPosition_ / glm::vec2(windowSize) - 1.0f;
}

glm::vec2 SMouse::CursorPositionDelta() const
{
    return cursorPosition_ - previousCursorPosition_;
}

float SMouse::HorizontalScrollDelta() const
{
    return scrollDelta_.x;
}

float SMouse::VerticalScrollDelta() const
{
    return scrollDelta_.y;
}

UEvent<>& SMouse::EventButton(const EButton button, const EInputState inputState)
{
    return buttonEvents_[to_index(button)][to_index(inputState)];
}

bool SMouse::ButtonState(const EButton button, const EInputState inputState) const
{
    return buttonStates_[to_index(button)][to_index(inputState)];
}

UEvent<glm::vec2>& SMouse::EventMove()
{
    return eventMove_;
}

UEvent<glm::vec2>& SMouse::EventScroll()
{
    return eventScroll_;
}

UEvent<float>& SMouse::EventHorizontalScroll()
{
    return eventHorizontalScroll_;
}

UEvent<float>& SMouse::EventVerticalScroll()
{
    return eventVerticalScroll_;
}

void mousebutton_callback_(GLFWwindow* window, int button, int action, int mods)
{
    Mouse.UpdateButton(static_cast<EButton>(button), action);
}

void cursorpos_callback_(GLFWwindow* window, double xpos, double ypos)
{
    Mouse.cursorPosition_ = { xpos, ypos };
}

void scroll_callback_(GLFWwindow* window, double xoffset, double yoffset)
{
    Mouse.scrollDelta_.x += static_cast<float>(xoffset);
    Mouse.scrollDelta_.y += static_cast<float>(yoffset);
}
