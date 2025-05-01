#include "Mouse.h"
#include "Framework.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include "log.h"

void KtMouse::Update()
{
    for (auto& [button, events] : _buttonEvents)
    {
        std::unordered_set<KtInputState> buttonStates;

        int glfwKeyState = glfwGetMouseButton(Framework.GetWindow().GetGLFWWindow(), button);
        switch (glfwKeyState)
        {
        case GLFW_PRESS:
        {
            buttonStates.insert(KT_INPUT_STATE_DOWN);
            if (!_buttonStates[button].contains(KT_INPUT_STATE_DOWN))
            {
                buttonStates.insert(KT_INPUT_STATE_PRESSED);
            }
            break;
        }
        case GLFW_RELEASE:
        {
            buttonStates.insert(KT_INPUT_STATE_UP);
            if (!_buttonStates[button].contains(KT_INPUT_STATE_UP))
            {
                buttonStates.insert(KT_INPUT_STATE_RELEASED);
            }
            break;
        }
        default:
            break;
        }

        _buttonStates[button] = buttonStates;

        for (const auto inputState : _buttonStates[button])
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
    _previousCursorPosition = _cursorPosition;
    _cursorPosition = newPos;

    if (_cursorPosition != newPos)
    {
        _moveEvent.Broadcast();
    }
}

const glm::vec2& KtMouse::GetPreviousCursorPosition() const
{
    return _previousCursorPosition;
}

const glm::vec2& KtMouse::GetCursorPosition() const
{
    return _cursorPosition;
}

const glm::vec2 KtMouse::GetCursorPositionNormalized() const
{
    const auto& windowSize = Framework.GetWindow().GetSize();
    return 2.0f * _cursorPosition / glm::vec2(windowSize) - 1.0f;
}

const glm::vec2 KtMouse::GetCursorPositionDelta() const
{
    return _cursorPosition - _previousCursorPosition;
}

KtEvent<>& KtMouse::GetButtonEvent(const KtButton button, const KtInputState inputState)
{
    return _buttonEvents[button][inputState];
}

KtEvent<>& KtMouse::GetMoveEvent()
{
    return _moveEvent;
}
