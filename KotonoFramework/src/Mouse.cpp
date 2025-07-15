#include "Mouse.h"
#include "Framework.h"
#include "Window.h"
#include "InputManager.h"
#include <GLFW/glfw3.h>

void mousebutton_callback_(GLFWwindow* window, int button, int action, int mods);
void cursorpos_callback_(GLFWwindow* window, double xpos, double ypos);

void KtMouse::Init()
{
    glfwSetMouseButtonCallback(Framework.GetWindow().GetGLFWWindow(), mousebutton_callback_);
    glfwSetCursorPosCallback(Framework.GetWindow().GetGLFWWindow(), cursorpos_callback_);
}

void KtMouse::Update()
{
    for (size_t button = 0; button < buttonStates_.size(); button++)
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
        moveEvent_.Broadcast();
        previousCursorPosition_ = cursorPosition_;
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

void KtMouse::UpdateCursor(const glm::vec2& position)
{
    cursorPosition_ = position;
}

const glm::vec2& KtMouse::GetPreviousCursorPosition() const
{
    return previousCursorPosition_;
}

const glm::vec2& KtMouse::GetCursorPosition() const
{
    return cursorPosition_;
}

const glm::vec2 KtMouse::GetCursorPositionNormalized() const
{
    const auto& windowSize = Framework.GetWindow().GetSize();
    return 2.0f * cursorPosition_ / glm::vec2(windowSize) - 1.0f;
}

const glm::vec2 KtMouse::GetCursorPositionDelta() const
{
    return cursorPosition_ - previousCursorPosition_;
}

KtEvent<>& KtMouse::GetButtonEvent(const KtButton button, const KtInputState inputState)
{
    return buttonEvents_[button][inputState];
}

KtEvent<>& KtMouse::GetMoveEvent()
{
    return moveEvent_;
}

void mousebutton_callback_(GLFWwindow* window, int button, int action, int mods)
{
    Framework.GetInputManager().GetMouse().UpdateButton(static_cast<KtButton>(button), action);
}

void cursorpos_callback_(GLFWwindow* window, double xpos, double ypos)
{
    Framework.GetInputManager().GetMouse().UpdateCursor(glm::vec2(xpos, ypos));
}
