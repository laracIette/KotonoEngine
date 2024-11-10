#include "Rect.h"
#include <glm/gtc/matrix_transform.hpp>

Rect::Rect() :
    _baseSize(glm::vec2(0.0f, 0.0f)),
    _relativePosition(glm::vec2(0.0f)),
    _relativeRotation(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f))),
    _relativeScale(glm::vec2(1.0f, 1.0f)),
    _parent(nullptr)
{
}

const glm::vec2& Rect::GetBaseSize() const
{
    return _baseSize;
}

const glm::vec2& Rect::GetRelativePosition() const
{
    return _relativePosition;
}

const glm::quat& Rect::GetRelativeRotation() const
{
    return _relativeRotation;
}

const glm::vec2& Rect::GetRelativeScale() const
{
    return _relativeScale;
}

const glm::vec2 Rect::GetRelativeSize() const
{
    return _baseSize * _relativeScale;
}

const glm::vec2 Rect::GetWorldPosition() const
{
    if (_parent)
    {
        return _relativePosition + _parent->GetWorldPosition();
    }
    return _relativePosition;
}

const glm::quat Rect::GetWorldRotation() const
{
    if (_parent)
    {
        return _relativeRotation + _parent->GetWorldRotation();
    }
    return _relativeRotation;
}

const glm::vec2 Rect::GetWorldScale() const
{
    if (_parent)
    {
        return _relativeScale * _parent->GetWorldScale();
    }
    return _relativeScale;
}

const glm::vec2 Rect::GetWorldSize() const
{
    return _baseSize * GetWorldScale();
}

const glm::mat4 Rect::GetModelMatrix() const
{
    constexpr glm::vec2 windowSize(1600, 900);

    glm::vec2 worldPosition = GetWorldPosition();

    glm::vec3 ndcPosition(
        2.0f * worldPosition.x / windowSize.x - 1.0f,
        1.0f - worldPosition.y / windowSize.y * 2.0f,
        0.0f
    );
    glm::vec3 ndcSize(GetWorldSize() / windowSize, 1.0f);

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), ndcPosition);
    glm::mat4 rotationMatrix = glm::mat4_cast(GetWorldRotation());
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), ndcSize);

    return translationMatrix * rotationMatrix * scaleMatrix;
}

Rect* Rect::GetParent() const
{
    return _parent;
}

void Rect::SetBaseSize(const glm::vec2& baseSize)
{
    _baseSize = baseSize;
}

void Rect::SetRelativePosition(const glm::vec2& relativePosition)
{
    _relativePosition = relativePosition;
}

void Rect::SetRelativeRotation(const glm::quat& relativeRotation)
{
    _relativeRotation = relativeRotation;
}

void Rect::SetRelativeScale(const glm::vec2& relativeScale)
{
    _relativeScale = relativeScale;
}

void Rect::SetRelativeSize(const glm::vec2& relativeSize)
{
    if (_baseSize == glm::vec2(0.0f, 0.0f))
    {
        _baseSize = relativeSize / _relativeScale;
        return;
    }
    _relativeScale = relativeSize / _baseSize;
}

void Rect::SetWorldPosition(const glm::vec2& worldPosition)
{
    if (_parent)
    {
        _relativePosition = worldPosition - _parent->GetWorldPosition();
        return;
    }
    _relativePosition = worldPosition;
}

void Rect::SetWorldRotation(const glm::quat& worldRotation)
{
    if (_parent)
    {
        _relativeRotation = worldRotation - _parent->GetWorldRotation();
        return;
    }
    _relativeRotation = worldRotation;
}

void Rect::SetWorldScale(const glm::vec2& worldScale)
{
    if (_parent)
    {
        _relativeScale = worldScale / _parent->GetWorldScale();
        return;
    }
    _relativeScale = worldScale;
}

void Rect::SetWorldSize(const glm::vec2& worldSize)
{
    if (_baseSize == glm::vec2(0.0f, 0.0f))
    {
        _baseSize = worldSize / GetWorldScale();
        return;
    }
    SetWorldScale(worldSize / _baseSize);
}

void Rect::SetParent(Rect* parent)
{
    _parent = parent;
}

void Rect::Update(float deltaTime)
{
}

const glm::vec2 Rect::GetDirection(Rect* target) const
{
    return target->GetWorldPosition() - GetWorldPosition();
}

const float Rect::GetDistance(Rect* other) const
{
    return GetDirection(other).length();
}
