#include "Mouse.h"
#include "Framework.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include "log.h"

void KtMouse::Update()
{
    for (auto& [button, events] : buttonEvents_)
    {
        std::unordered_set<KtInputState> buttonStates;

        int glfwKeyState = glfwGetMouseButton(Framework.GetWindow().GetGLFWWindow(), button);
        switch (glfwKeyState)
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
            buttonStates.insert(KT_INPUT_STATE_UP);
            if (!buttonStates_[button].contains(KT_INPUT_STATE_UP))
            {
                buttonStates.insert(KT_INPUT_STATE_RELEASED);
            }
            break;
        }
        default:
            break;
        }

        buttonStates_[button] = buttonStates;

        for (const auto inputState : buttonStates_[button])
        {
            if (events.contains(inputState))
            {
                events[inputState].Broadcast();
            }
        }
    }

    double x, y;
    glfwGetCursorPos(Framework.GetWindow().GetGLFWWindow(), &x, &y);
    const auto newPos = glm::vec2(x, y);
    previousCursorPosition_ = cursorPosition_;
    cursorPosition_ = newPos;

    if (cursorPosition_ != newPos)
    {
        moveEvent_.Broadcast();
    }
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
