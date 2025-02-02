#include "Mouse.h"
#include "KotonoFramework.h"

const glm::vec2& KtMouse::GetCursorPosition() const
{
    return _cursorPosition;
}

const glm::vec2 KtMouse::GetCursorPositionNormalized() const
{
    const glm::vec2 windowSize = Framework.GetWindow().GetSize();

    const float normalizedX = (2.0f * _cursorPosition.x / windowSize.x) - 1.0f;
    const float normalizedY = (2.0f * _cursorPosition.y / windowSize.y) - 1.0f;

    return glm::vec2(normalizedX, normalizedY);
}

void KtMouse::SetCursorPosition(const glm::vec2& position)
{
    _cursorPosition = position;
}
