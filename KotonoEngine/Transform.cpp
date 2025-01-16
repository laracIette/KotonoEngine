#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform() :
	_relativeLocation(0.0f, 0.0f, 0.0f), 
	_relativeRotation(glm::vec3(0.0f, 0.0f, 0.0f)),
	_relativeScale(1.0f, 1.0f, 1.0f),
	_parent(nullptr)
{
}

const glm::vec3& Transform::GetRelativeLocation() const
{
	return _relativeLocation;
}

const glm::quat& Transform::GetRelativeRotation() const
{
	return _relativeRotation;
}

const glm::vec3& Transform::GetRelativeScale() const
{
	return _relativeScale;
}

const glm::vec3 Transform::GetWorldLocation() const
{
	if (_parent)
	{
		return _relativeLocation + _parent->GetWorldLocation();
	}
	return _relativeLocation;
}

const glm::quat Transform::GetWorldRotation() const
{
	if (_parent)
	{
		return _relativeRotation + _parent->GetWorldRotation();
	}
	return _relativeRotation;
}

const glm::vec3 Transform::GetWorldScale() const
{
	if (_parent)
	{
		return _relativeScale * _parent->GetWorldScale();
	}
	return _relativeScale;
}

const glm::mat4 Transform::GetModelMatrix() const
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), GetWorldLocation());
	glm::mat4 rotationMatrix = glm::mat4_cast(GetWorldRotation());
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), GetWorldScale());

	return translationMatrix * rotationMatrix * scaleMatrix;
}

Transform* Transform::GetParent() const
{
	return _parent;
}

void Transform::SetRelativeLocation(const glm::vec3& relativeLocation)
{
	_relativeLocation = relativeLocation;
}

void Transform::SetRelativeRotation(const glm::quat& relativeRotation)
{
	_relativeRotation = relativeRotation;
}

void Transform::SetRelativeScale(const glm::vec3& relativeScale)
{
	_relativeScale = relativeScale;
}

void Transform::SetWorldLocation(const glm::vec3& worldLocation)
{
	if (_parent)
	{
		_relativeLocation = worldLocation - _parent->GetWorldLocation();
		return;
	}
	_relativeLocation = worldLocation;
}

void Transform::SetWorldRotation(const glm::quat& worldRotation)
{
	if (_parent)
	{
		_relativeRotation = worldRotation - _parent->GetWorldRotation();
		return;
	}
	_relativeRotation = worldRotation;
}

void Transform::SetWorldScale(const glm::vec3& worldScale)
{
	if (_parent)
	{
		_relativeScale = worldScale / _parent->GetWorldScale();
		return;
	}
	_relativeScale = worldScale;
}

void Transform::SetParent(Transform* parent)
{
	_parent = parent;
}

const glm::vec3 Transform::GetDirection(Transform* target) const
{
	return target->GetWorldLocation() - GetWorldLocation();
}

const float Transform::GetDistance(Transform* other) const
{
	return GetDirection(other).length();
}

