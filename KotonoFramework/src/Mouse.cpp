#include "Mouse.h"
#include "Framework.h"
#include "Window.h"
#include "InputManager.h"
#include <GLFW/glfw3.h>

void mousebutton_callback_(GLFWwindow* window, int button, int action, int mods);
void cursorpos_callback_(GLFWwindow* window, double xpos, double ypos);
void scroll_callback_(GLFWwindow* window, double xoffset, double yoffset);

void KtMouse::Init()
{
    glfwSetMouseButtonCallback(Framework.Window().GetGLFWWindow(), mousebutton_callback_);
    glfwSetCursorPosCallback(Framework.Window().GetGLFWWindow(), cursorpos_callback_);
    glfwSetScrollCallback(Framework.Window().GetGLFWWindow(), scroll_callback_);
}

void KtMouse::Update()
{
    for (size_t button{ 0 }; button < buttonStates_.size(); ++button)
    {
        if (buttonStates_[button].empty())
        {
            continue;
        }

        for (const auto inputState : buttonStates_[button])
        {
            buttonEvents_[button][inputState].Broadcast();
        }

        std::erase_if(buttonStates_[button],
            [](const KtInputState inputState) 
            {
                return inputState == KT_INPUT_STATE_PRESSED || inputState == KT_INPUT_STATE_RELEASED;
            }
        );
    }

    if (cursorPosition_ != previousCursorPosition_)
    {
        eventMove_.Broadcast(GetCursorPositionDelta());
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

void KtMouse::UpdateButton(const KtButton button, const int action)
{
    std::unordered_set<KtInputState> buttonStates;

    switch (action)
    {
    case GLFW_PRESS:
    {
        buttonStates.insert(KT_INPUT_STATE_DOWN);
        if (!buttonStates_[button].contains(KT_INPUT_STATE_DOWN))
        {
            buttonStates.insert(KT_INPUT_STATE_PRESSED);
        }
        break;
    }
    case GLFW_RELEASE:
    {
        buttonStates.insert(KT_INPUT_STATE_RELEASED);
        break;
    }
    default:
        break;
    }

    buttonStates_[button] = buttonStates;
}

const glm::vec2& KtMouse::GetPreviousCursorPosition() const
{
    return previousCursorPosition_;
}

const glm::vec2& KtMouse::GetCursorPosition() const
{
    return cursorPosition_;
}

glm::vec2 KtMouse::GetCursorPositionNormalized() const
{
    const auto& windowSize = Framework.Window().GetSize();
    return 2.0f * cursorPosition_ / glm::vec2(windowSize) - 1.0f;
}

glm::vec2 KtMouse::GetCursorPositionDelta() const
{
    return cursorPosition_ - previousCursorPosition_;
}

float KtMouse::GetHorizontalScrollDelta() const
{
    return horizontalScrollDelta_;
}

float KtMouse::GetVerticalScrollDelta() const
{
    return verticalScrollDelta_;
}

KtEvent<>& KtMouse::ButtonEvent(const KtButton button, const KtInputState inputState)
{
    return buttonEvents_[button][inputState];
}

KtEvent<glm::vec2>& KtMouse::EventMove()
{
    return eventMove_;
}

KtEvent<float>& KtMouse::EventHorizontalScroll()
{
    return eventHorizontalScroll_;
}

KtEvent<float>& KtMouse::EventVerticalScroll()
{
    return eventVerticalScroll_;
}

void mousebutton_callback_(GLFWwindow* window, int button, int action, int mods)
{
    Framework.InputManager().Mouse().UpdateButton(static_cast<KtButton>(button), action);
}

void cursorpos_callback_(GLFWwindow* window, double xpos, double ypos)
{
    Framework.InputManager().Mouse().cursorPosition_ = { xpos, ypos };
}

void scroll_callback_(GLFWwindow* window, double xoffset, double yoffset)
{
    Framework.InputManager().Mouse().horizontalScrollDelta_ = static_cast<float>(xoffset);
    Framework.InputManager().Mouse().verticalScrollDelta_ = static_cast<float>(yoffset);
}
