#include "Mouse.h"
#include "KotonoEngine.h"

const glm::vec2& Mouse::GetCursorPosition() const
{
    return _cursorPosition;
}

const glm::vec2 Mouse::GetCursorPositionNormalized() const
{
    const glm::vec2 windowSize = Engine->GetWindow()->GetSize();

    const float normalizedX = (2.0f * _cursorPosition.x / windowSize.x) - 1.0f;
    const float normalizedY = (2.0f * _cursorPosition.y / windowSize.y) - 1.0f;

    return glm::vec2(normalizedX, normalizedY);
}

void Mouse::SetCursorPosition(const glm::vec2& position)
{
    _cursorPosition = position;
}
