#include "Transform.h"

UTransform::UTransform() :
	_relativeLocation(0.0f, 0.0f, 0.0f),
	_relativeRotation(glm::identity<glm::quat>()),
	_relativeScale(1.0f, 1.0f, 1.0f),
	_parent(nullptr)
{
}

const glm::vec3& UTransform::GetRelativeLocation() const
{
	return _relativeLocation;
}

const glm::quat& UTransform::GetRelativeRotation() const
{
	return _relativeRotation;
}

const glm::vec3& UTransform::GetRelativeScale() const
{
	return _relativeScale;
}

const glm::vec3 UTransform::GetWorldLocation() const
{
	if (_parent)
	{
		return _parent->GetWorldLocation() * _relativeLocation;
	}
	return _relativeLocation;
}

const glm::quat UTransform::GetWorldRotation() const
{
	if (_parent)
	{
		return _parent->GetWorldRotation() * _relativeRotation;
	}
	return _relativeRotation;
}

const glm::vec3 UTransform::GetWorldScale() const
{
	if (_parent)
	{
		return _parent->GetWorldScale() * _relativeScale;
	}
	return _relativeScale;
}

const glm::vec3 UTransform::GetRightVector() const
{
	return GetWorldRotation() * glm::vec3(1.0f, 0.0f, 0.0f);
}

const glm::vec3 UTransform::GetForwardVector() const
{
	return GetWorldRotation() * glm::vec3(0.0f, 1.0f, 0.0f);
}

const glm::vec3 UTransform::GetUpVector() const
{
	return GetWorldRotation() * glm::vec3(0.0f, 0.0f, 1.0f);;
}

UTransform* UTransform::GetParent() const
{
	return _parent;
}

void UTransform::SetRelativeLocation(const glm::vec3& relativeLocation)
{
	_relativeLocation = relativeLocation;
}

void UTransform::SetRelativeRotation(const glm::quat& relativeRotation)
{
	_relativeRotation = relativeRotation;
}

void UTransform::SetRelativeScale(const glm::vec3& relativeScale)
{
	_relativeScale = relativeScale;
}

void UTransform::SetWorldLocation(const glm::vec3& worldLocation)
{
	if (_parent)
	{
		_relativeLocation = worldLocation - _parent->GetWorldLocation();
		return;
	}
	_relativeLocation = worldLocation;
}

void UTransform::SetWorldRotation(const glm::quat& worldRotation)
{
	if (_parent)
	{
		_relativeRotation = glm::inverse(_parent->GetWorldRotation()) * worldRotation;
		return;
	}
	_relativeRotation = worldRotation;
}

void UTransform::SetWorldScale(const glm::vec3& worldScale)
{
	if (_parent)
	{
		_relativeScale = worldScale / _parent->GetWorldScale();
		return;
	}
	_relativeScale = worldScale;
}

void UTransform::SetParent(UTransform* parent)
{
	_parent = parent;
}

const glm::mat4 UTransform::GetModelMatrix() const
{
	const glm::mat4 translationMatrix = glm::translate(glm::identity<glm::mat4>(), GetWorldLocation());
	const glm::mat4 rotationMatrix = glm::mat4_cast(GetWorldRotation());
	const glm::mat4 scaleMatrix = glm::scale(glm::identity<glm::mat4>(), GetWorldScale());

	return translationMatrix * rotationMatrix * scaleMatrix;
}

const glm::vec3 UTransform::GetDirection(UTransform* target) const
{
	return target->GetWorldLocation() - GetWorldLocation();
}

const float UTransform::GetDistance(UTransform* other) const
{
	return glm::distance(GetWorldLocation(), other->GetWorldLocation());
}

