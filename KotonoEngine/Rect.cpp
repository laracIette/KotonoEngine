#include "Rect.h"
#include "NDCRect.h"
#include <format>
#include "glm_helper.h"

Rect::Rect() :
    _baseSize(0.0f, 0.0f),
    _anchor(ANCHOR_CENTER),
    _relativePosition(0.0f, 0.0f),
    _relativeRotation(),
    _relativeScale(1.0f, 1.0f),
    _relativePositionVelocity(0.0f, 0.0f),
    _parent(nullptr)
{
}

const glm::vec2& Rect::GetBaseSize() const
{
    return _baseSize;
}

const Anchor Rect::GetAnchor() const
{
    return _anchor;
}

const glm::vec2 Rect::GetRelativePosition() const
{
    return _relativePosition + GetAnchorPositionDelta();
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
        return GetRelativePosition() + _parent->GetWorldPosition();
    }
    return GetRelativePosition();
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

const glm::vec2& Rect::GetRelativePositionVelocity() const
{
    return _relativePositionVelocity;
}

const glm::mat4 Rect::GetModelMatrix() const
{
    return NDCRect(GetWorldPosition(), GetWorldRotation(), GetWorldSize()).GetModelMatrix();
}

Rect* Rect::GetParent() const
{
    return _parent;
}

void Rect::SetBaseSize(const glm::vec2& baseSize)
{
    _baseSize = baseSize;
}

void Rect::SetAnchor(const Anchor anchor)
{
    _anchor = anchor;
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

void Rect::SetRelativePositionVelocity(const glm::vec2& relativePositionVelocity)
{
    _relativePositionVelocity = relativePositionVelocity;
}

void Rect::SetParent(Rect* parent)
{
    _parent = parent;
}

void Rect::Update(float deltaTime)
{
    _relativePosition += _relativePositionVelocity * deltaTime;
}

const glm::vec2 Rect::GetDirection(Rect* target) const
{
    return target->GetWorldPosition() - GetWorldPosition();
}

const float Rect::GetDistance(Rect* other) const
{
    return static_cast<float>(GetDirection(other).length());
}

const bool Rect::Overlaps(Rect* other) const
{
    return Overlaps(other->GetWorldPosition(), other->GetWorldSize());
}

const bool Rect::Overlaps(const glm::vec2& position) const
{
    glm::vec2 difference = glm::abs(GetWorldPosition() - position);
    glm::vec2 halfSize = GetWorldSize() / 2.0f;
    return difference.x < halfSize.x && difference.y < halfSize.y;
}

const bool Rect::Overlaps(const glm::vec2& position, const glm::vec2& size) const
{
    glm::vec2 difference = glm::abs(GetWorldPosition() - position);
    glm::vec2 halfSumOfSize = (GetWorldSize() + size) / 2.0f;
    return difference.x < halfSumOfSize.x && difference.y < halfSumOfSize.y;
}

const std::string Rect::ToString() const
{
    return "World: {Position: {" + glm::to_string(GetWorldPosition())
        + "}, Rotation: {" + glm::to_string(GetWorldRotation())
        + "}, Size: {" + glm::to_string(GetWorldSize()) + "}}";
}

const glm::vec2 Rect::GetAnchorPositionDelta() const
{
    return glm::vec2(
        (_anchor & ANCHOR_LEFT) == ANCHOR_LEFT ? GetRelativeSize().x / 2.0f
        : (_anchor & ANCHOR_RIGHT) == ANCHOR_RIGHT ? -GetRelativeSize().x / 2.0f
        : 0.0f,
        (_anchor & ANCHOR_TOP) == ANCHOR_TOP ? GetRelativeSize().y / 2.0f
        : (_anchor & ANCHOR_BOTTOM) == ANCHOR_BOTTOM ? -GetRelativeSize().y / 2.0f
        : 0.0f
    );
}
