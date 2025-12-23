#include "Mouse.h"
#include <GLFW/glfw3.h>
#include <kotono_common/log.h> 
#include <kotono_platform/Window.h>

#define KT_LOG_IMPORTANCE_LEVEL_MOUSE KT_LOG_IMPORTANCE_LEVEL_LOW

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
    for (size_t button{ 0 }; button < KT_BUTTON_COUNT; ++button)
    {
        for (size_t inputState{ 0 }; inputState < KT_INPUT_STATE_COUNT; ++inputState)
        {
            if (buttonStates_[button][inputState])
            {
                buttonEvents_[button][inputState].Broadcast();
            }
        }

        if (buttonStates_[button][KT_INPUT_STATE_PRESSED])
        {
            buttonStates_[button][KT_INPUT_STATE_PRESSED] = false;
        }
    }

    if (cursorPosition_ != previousCursorPosition_)
    {
        eventMove_.Broadcast(CursorPositionDelta());
        previousCursorPosition_ = cursorPosition_;
    }

    if (horizontalScrollDelta_ != 0.0f)
    {
        eventHorizontalScroll_.Broadcast(horizontalScrollDelta_);
        horizontalScrollDelta_ = 0.0f;
    }

    if (verticalScrollDelta_ != 0.0f)
    {
        eventVerticalScroll_.Broadcast(verticalScrollDelta_);
        verticalScrollDelta_ = 0.0f;
    }
}

void SMouse::UpdateButton(const KtButton button, const int action)
{
    switch (action)
    {
    case GLFW_PRESS:
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_MOUSE, "Input.SMouse::UpdateButton()", "GLFW_PRESS button %d", button);

        buttonStates_[button][KT_INPUT_STATE_RELEASED] = false;

        buttonStates_[button][KT_INPUT_STATE_PRESSED] = true;
        buttonStates_[button][KT_INPUT_STATE_DOWN] = true;
        break;
    }
    case GLFW_RELEASE:
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_MOUSE, "Input.SMouse::UpdateButton()", "GLFW_RELEASE button %d", button);
        
        buttonStates_[button][KT_INPUT_STATE_PRESSED] = false;
        buttonStates_[button][KT_INPUT_STATE_DOWN] = false;

        buttonStates_[button][KT_INPUT_STATE_RELEASED] = true;
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
    return horizontalScrollDelta_;
}

float SMouse::VerticalScrollDelta() const
{
    return verticalScrollDelta_;
}

KtEvent<>& SMouse::EventButton(const KtButton button, const KtInputState inputState)
{
    return buttonEvents_[button][inputState];
}

bool SMouse::ButtonState(const KtButton button, const KtInputState inputState) const
{
    return buttonStates_[button][inputState];
}

KtEvent<glm::vec2>& SMouse::EventMove()
{
    return eventMove_;
}

KtEvent<float>& SMouse::EventHorizontalScroll()
{
    return eventHorizontalScroll_;
}

KtEvent<float>& SMouse::EventVerticalScroll()
{
    return eventVerticalScroll_;
}

void mousebutton_callback_(GLFWwindow* window, int button, int action, int mods)
{
    Mouse.UpdateButton(static_cast<KtButton>(button), action);
}

void cursorpos_callback_(GLFWwindow* window, double xpos, double ypos)
{
    Mouse.cursorPosition_ = { xpos, ypos };
}

void scroll_callback_(GLFWwindow* window, double xoffset, double yoffset)
{
    Mouse.horizontalScrollDelta_ = static_cast<float>(xoffset);
    Mouse.verticalScrollDelta_ = static_cast<float>(yoffset);
}
