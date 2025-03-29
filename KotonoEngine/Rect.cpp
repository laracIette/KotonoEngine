#include "Rect.h"
#include <kotono_framework/Viewport.h>

URect::URect(): 
	_relativePosition(0.0f, 0.0f),
	_relativeRotation(0.0f),
	_relativeScale(1.0f, 1.0f),
	_baseSize(0.0f, 0.0f),
	_parent(nullptr)
{
}

const glm::vec2& URect::GetRelativePosition() const
{
	return _relativePosition;
}

const float URect::GetRelativeRotation() const
{
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

const float URect::GetWorldRotation() const
{
	if (_parent)
	{
		return _relativeRotation + _parent->GetWorldRotation();
	}
	return _relativeRotation;
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

void URect::SetRelativeRotation(const float relativeRotation)
{
	_relativeRotation = relativeRotation;
}

void URect::SetRelativeScale(const glm::vec2& relativeScale)
{
	_relativeScale = relativeScale;
}

void URect::SetWorldPosition(const glm::vec2& worldPosition)
{
	if (_parent)
	{
		_relativePosition = worldPosition - _parent->GetWorldPosition();
		return;
	}
	_relativePosition = worldPosition;
}

void URect::SetWorldRotation(const float worldRotation)
{
	if (_parent)
	{
		_relativeRotation = worldRotation - _parent->GetWorldRotation();
		return;
	}
	_relativeRotation = worldRotation;
}

void URect::SetWorldScale(const glm::vec2& worldScale)
{
	if (_parent)
	{
		_relativeScale = worldScale / _parent->GetWorldScale();
		return;
	}
	_relativeScale = worldScale;
}

void URect::SetParent(URect* parent)
{
	_parent = parent;
}

void URect::SetBaseSize(const glm::uvec2& baseSize)
{
	_baseSize = baseSize;
}

const glm::mat4 URect::GetModelMatrix() const
{
	const auto& viewportExtent = WindowViewport.GetExtent();
	const auto viewportSize = glm::vec2(viewportExtent.width, viewportExtent.height);
	const auto size = glm::vec2(_baseSize) * GetWorldScale() / viewportSize;

	const glm::mat4 translationMatrix = glm::translate(glm::identity<glm::mat4>(), glm::vec3(GetWorldPosition(), 0.0f));
	const glm::mat4 rotationMatrix = glm::rotate(glm::identity<glm::mat4>(), GetWorldRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
	const glm::mat4 scaleMatrix = glm::scale(glm::identity<glm::mat4>(), glm::vec3(size, 1.0f));

	return translationMatrix * rotationMatrix * scaleMatrix;
}