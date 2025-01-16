#include "EngineInterfaceRect.h"

const glm::vec2& EngineInterfaceRect::GetPosition() const
{
    return _position;
}

const glm::vec2& EngineInterfaceRect::GetSize() const
{
    return _size;
}

void EngineInterfaceRect::SetPosition(const glm::vec2& position)
{
    _position = position;
}

void EngineInterfaceRect::SetSize(const glm::vec2& size)
{
    _size = size;
}

const bool EngineInterfaceRect::Overlaps(const glm::vec2& position) const
{
    const glm::vec2 difference = glm::abs(_position - position);
    const glm::vec2 halfSize = _size / 2.0f;
    return difference.x < halfSize.x && difference.y < halfSize.y;
}

const Edge EngineInterfaceRect::GetClosestEdge(const glm::vec2& position) const
{
    constexpr float LEFT(-1.0f);
    constexpr float RIGHT(1.0f);
    constexpr float TOP(-1.0f);
    constexpr float BOTTOM(1.0f);

    const glm::vec2 relativePosition = position - GetPosition();
    const glm::vec2 normalizedPosition = relativePosition / GetSize();

    const float distanceLeft = std::abs(LEFT - normalizedPosition.x);
    const float distanceRight = std::abs(RIGHT - normalizedPosition.x);
    const float distanceTop = std::abs(TOP - normalizedPosition.y);
    const float distanceBottom = std::abs(BOTTOM - normalizedPosition.y);

    float minDistance = distanceLeft;
    Edge closestEdge = EDGE_LEFT;

    if (distanceRight < minDistance)
    {
        minDistance = distanceRight;
        closestEdge = EDGE_RIGHT;
    }
    if (distanceTop < minDistance)
    {
        minDistance = distanceTop;
        closestEdge = EDGE_TOP;
    }
    if (distanceBottom < minDistance)
    {
        minDistance = distanceBottom;
        closestEdge = EDGE_BOTTOM;
    }

    return closestEdge;
}
