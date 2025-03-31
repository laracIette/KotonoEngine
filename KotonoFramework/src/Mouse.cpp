#include "Mouse.h"
#include "Framework.h"

void KtMouse::Update()
{
    for (auto& [button, events] : _events)
    {
        std::unordered_set<KtInputState> buttonStates;

        int glfwKeyState = glfwGetMouseButton(Framework.GetWindow().GetGLFWWindow(), button);
        switch (glfwKeyState)
        {
        case GLFW_PRESS:
            buttonStates.insert(KT_INPUT_STATE_DOWN);
            if (!_buttonStates[button].contains(KT_INPUT_STATE_DOWN))
            {
                buttonStates.insert(KT_INPUT_STATE_PRESSED);
            }
            break;

        case GLFW_RELEASE:
            buttonStates.insert(KT_INPUT_STATE_UP);
            if (!_buttonStates[button].contains(KT_INPUT_STATE_UP))
            {
                buttonStates.insert(KT_INPUT_STATE_RELEASED);
            }
            break;

        case GLFW_REPEAT:
            buttonStates.insert(KT_INPUT_STATE_DOWN);
            buttonStates.insert(KT_INPUT_STATE_REPEATED);
            break;

        default:
            break;
        }

        _buttonStates[button] = buttonStates;

        for (auto inputState : _buttonStates[button])
        {
            if (events.contains(inputState))
            {
                events[inputState].Broadcast();
            }
        }
    }

    double x, y;
    glfwGetCursorPos(Framework.GetWindow().GetGLFWWindow(), &x, &y);
    auto newPos = glm::vec2(x, y);
    if (_cursorPosition != newPos)
    {
        _previousCursorPosition = _cursorPosition;
        _cursorPosition = newPos;
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
    const glm::uvec2 windowSize = Framework.GetWindow().GetSize();

    const float normalizedX = (2.0f * _cursorPosition.x / windowSize.x) - 1.0f;
    const float normalizedY = (2.0f * _cursorPosition.y / windowSize.y) - 1.0f;

    return glm::vec2(normalizedX, normalizedY);
}

const glm::vec2 KtMouse::GetCursorPositionDelta() const
{
    return _cursorPosition - _previousCursorPosition;
}

KtEvent& KtMouse::GetEvent(const KtButton button, const KtInputState inputState)
{
    return _events[button][inputState];
}

KtEvent& KtMouse::GetMoveEvent()
{
    return _moveEvent;
}
