#include "Rect.h"
#include <kotono_framework/Viewport.h>
#include <kotono_framework/log.h>

URect::URect(): 
	_relativePosition(0.0f, 0.0f),
	_relativeRotation(0.0f),
	_relativeScale(1.0f, 1.0f),
	_baseSize(0.0f, 0.0f),
	_anchor(EAnchor::Center),
	_parent(nullptr)
{
}

const glm::vec2& URect::GetRelativePosition() const
{
	return _relativePosition;
}

const float URect::GetRelativeRotation(const ERotationUnit unit) const
{
	if (unit == ERotationUnit::Degrees)
	{
		return glm::degrees(_relativeRotation);
	}
	return _relativeRotation;
}

const glm::vec2& URect::GetRelativeScale() const
{
	return _relativeScale;
}

const glm::vec2 URect::GetWorldPosition() const
{
	if (_parent)
	{
		return _relativePosition + _parent->GetWorldPosition();
	}
	return _relativePosition;
}

const glm::vec2 URect::GetWorldPositionWithAnchorOffset() const
{
	if (_parent)
	{
		return _relativePosition + GetAnchorOffset() + _parent->GetWorldPositionWithAnchorOffset();
	}
	return _relativePosition + GetAnchorOffset();
}

const glm::vec2 URect::GetAnchorRelativePosition() const
{
	return _relativePosition - GetAnchorOffset();
}

const bool URect::GetIsOverlapping(const glm::vec2& screenPosition, const glm::vec2& screenSize) const
{
	const auto distance = glm::abs(GetScreenPosition() - screenPosition);
	const auto maxDistance = (GetScreenSize() + screenSize) / 2.0f;
	return distance.x < maxDistance.x
		&& distance.y < maxDistance.y;
}

const float URect::GetWorldRotation(const ERotationUnit unit) const
{
	if (_parent)
	{
		return GetRelativeRotation(unit) + _parent->GetWorldRotation(unit);
	}
	return GetRelativeRotation(unit);
}

const glm::vec2 URect::GetScreenPosition() const
{
	const auto& viewportExtent = WindowViewport.GetExtent();
	const auto viewportSize = glm::vec2(viewportExtent.width, viewportExtent.height);
	const auto newPosition = (GetWorldPosition() + glm::vec2(1.0f)) * viewportSize / 2.0f;
	return newPosition;
}

const glm::vec2 URect::GetScreenSize() const
{
	return glm::vec2(_baseSize) * GetWorldScale();
}

const EAnchor URect::GetAnchor() const
{
	return _anchor;
}

const glm::vec2 URect::GetWorldScale() const
{
	if (_parent)
	{
		return _relativeScale * _parent->GetWorldScale();
	}
	return _relativeScale;
}

const glm::uvec2& URect::GetBaseSize() const
{
	return _baseSize;
}

URect* URect::GetParent() const
{
	return _parent;
}

void URect::SetRelativePosition(const glm::vec2& relativePosition)
{
	_relativePosition = relativePosition;
}

void URect::SetRelativeScale(const glm::vec2& relativeScale)
{
	_relativeScale = relativeScale;
}

void URect::SetRelativeRotation(float relativeRotation, const ERotationUnit unit)
{
	if (unit == ERotationUnit::Degrees)
	{
		relativeRotation = glm::radians(relativeRotation);
	}

	const glm::vec2 translated = GetAnchorOffset();

	const float cosA = cos(relativeRotation);
	const float sinA = sin(relativeRotation);
	const auto rotated = glm::vec2(
		translated.x * cosA - translated.y * sinA,
		translated.x * sinA + translated.y * cosA
	);

	_relativePosition = rotated + GetAnchorRelativePosition();
	_relativeRotation = relativeRotation;
}

void URect::SetWorldPosition(const glm::vec2& worldPosition)
{
	if (_parent)
	{
		SetRelativePosition(worldPosition - _parent->GetWorldPosition());
		return;
	}
	SetRelativePosition(worldPosition);
}

void URect::SetWorldScale(const glm::vec2& worldScale)
{
	if (_parent)
	{
		SetRelativeScale(worldScale / _parent->GetWorldScale());
		return;
	}
	SetRelativeScale(worldScale);
}

void URect::SetWorldRotation(const float worldRotation, const ERotationUnit unit)
{
	if (_parent)
	{
		SetRelativeRotation(worldRotation - _parent->GetWorldRotation(unit), unit);
		return;
	}
	SetRelativeRotation(worldRotation, unit);
}

void URect::AddOffset(const glm::vec2& offset)
{
	SetRelativePosition(_relativePosition + offset);
}

void URect::AddScale(const glm::vec2& scale)
{
	SetRelativeScale(_relativeScale * scale);
}

void URect::AddRotation(const float rotation, const ERotationUnit unit)
{
	SetRelativeRotation(GetRelativeRotation(unit) + rotation, unit);
}

void URect::SetScreenPosition(const glm::vec2& screenPosition)
{
	const auto& viewportExtent = WindowViewport.GetExtent();
	const auto viewportSize = glm::vec2(viewportExtent.width, viewportExtent.height);
	const auto newPosition = screenPosition / viewportSize * 2.0f - glm::vec2(1.0f);
	SetWorldPosition(newPosition);
}

void URect::SetScreenSize(const glm::vec2& screenSize)
{
	const auto& viewportExtent = WindowViewport.GetExtent();
	const auto viewportSize = glm::vec2(viewportExtent.width, viewportExtent.height);
	const auto newScale = screenSize / glm::vec2(_baseSize);
	SetWorldScale(newScale);
}

void URect::SetAnchor(const EAnchor anchor)
{
	_anchor = anchor;
}

void URect::SetParent(URect* parent, const ECoordinateSpace keepRect)
{
	switch (keepRect)
	{
	case ECoordinateSpace::Relative:
	{
		_parent = parent;
		break;
	}
	case ECoordinateSpace::World:
	{
		const URect clone = *this;
		_parent = parent;
		SetWorldPosition(clone.GetWorldPosition());
		SetWorldScale(clone.GetWorldScale());
		SetWorldRotation(clone.GetWorldRotation());
		break;
	}
	default:
		break;
	}
}

void URect::SetBaseSize(const glm::uvec2& baseSize)
{
	_baseSize = baseSize;
}

const glm::mat4 URect::GetTranslationMatrix() const
{
	return glm::translate(glm::identity<glm::mat4>(), glm::vec3(GetWorldPositionWithAnchorOffset(), 0.0f));
}

const glm::mat4 URect::GetRotationMatrix() const
{
	return glm::rotate(glm::identity<glm::mat4>(), GetWorldRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
}

const glm::mat4 URect::GetScaleMatrix() const
{
	const auto& viewportExtent = WindowViewport.GetExtent();
	const auto viewportSize = glm::vec2(viewportExtent.width, viewportExtent.height);
	const float aspectRatio = WindowViewport.GetAspectRatio();
	const float rotation = GetWorldRotation(ERotationUnit::Radians);

	// x *= 1 at rot 0
	// y *= 1 at rot 0
	// x *= aspectRatio at rot pi/2
	// y *= 1 / aspectRatio at rot pi/2

	const float value = (cos((rotation + glm::half_pi<float>()) * 2.0f) + 1.0f) / 2.0f;
	const auto stretchCorrection = glm::vec2(std::lerp(1.0f, aspectRatio, value), std::lerp(1.0f, 1.0f / aspectRatio, value));
	
	const auto size = GetScreenSize() / viewportSize * 2.0f * stretchCorrection;

	return glm::scale(glm::identity<glm::mat4>(), glm::vec3(size, 1.0f));
}

const glm::mat4 URect::GetModelMatrix() const
{
	return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
}

const glm::vec2 URect::GetDirection(const URect& target) const
{
	return target.GetWorldPosition() - GetWorldPosition();
}

const float URect::GetDistance(const URect& other) const
{
	return glm::distance(GetWorldPosition(), other.GetWorldPosition());
}

const bool URect::GetIsOverlapping(const glm::vec2& screenPosition) const
{
	return GetIsOverlapping(screenPosition, glm::vec2(0.0f));
}

const bool URect::GetIsOverlapping(const URect& other) const
{
	return GetIsOverlapping(other.GetScreenPosition(), other.GetScreenSize());
}

const glm::vec2 URect::GetAnchorOffset() const
{
	return glm::vec2(
		(_anchor & EAnchor::Left) == EAnchor::Left ? _relativeScale.x / 2.0f
		: (_anchor & EAnchor::Right) == EAnchor::Right ? -_relativeScale.x / 2.0f
		: 0.0f,
		(_anchor & EAnchor::Top) == EAnchor::Top ? _relativeScale.y / 2.0f
		: (_anchor & EAnchor::Bottom) == EAnchor::Bottom ? -_relativeScale.y / 2.0f
		: 0.0f
	);
}
