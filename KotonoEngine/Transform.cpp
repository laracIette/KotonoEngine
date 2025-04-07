#include "Transform.h"
#include <stdexcept>

UTransform::UTransform() :
	_relativePosition(0.0f, 0.0f, 0.0f),
	_relativeRotation(glm::identity<glm::quat>()),
	_relativeScale(1.0f, 1.0f, 1.0f),
	_parent(nullptr)
{
}

const glm::vec3& UTransform::GetRelativePosition() const
{
	return _relativePosition;
}

const glm::quat& UTransform::GetRelativeRotation() const
{
	return _relativeRotation;
}

const glm::vec3& UTransform::GetRelativeScale() const
{
	return _relativeScale;
}

const glm::vec3 UTransform::GetWorldPosition() const
{
	if (_parent)
	{
		return _parent->GetWorldPosition() + _relativePosition;
	}
	return _relativePosition;
}

const glm::quat UTransform::GetWorldRotation() const
{
	if (_parent)
	{
		return glm::normalize(_parent->GetWorldRotation() * _relativeRotation);
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
	return GetWorldRotation() * glm::vec3(0.0f, 0.0f, 1.0f);
}

UTransform* UTransform::GetParent() const
{
	return _parent;
}

void UTransform::SetRelativePosition(const glm::vec3& relativePosition)
{
	_relativePosition = relativePosition;
}

void UTransform::SetRelativeRotation(const glm::quat& relativeRotation)
{
	_relativeRotation = glm::normalize(relativeRotation);
}

void UTransform::SetRelativeScale(const glm::vec3& relativeScale)
{
	_relativeScale = relativeScale;
}

void UTransform::SetWorldPosition(const glm::vec3& worldPosition)
{
	if (_parent)
	{
		SetRelativePosition(worldPosition - _parent->GetWorldPosition());
		return;
	}
	SetRelativePosition(worldPosition);
}

void UTransform::SetWorldRotation(const glm::quat& worldRotation)
{
	if (_parent)
	{
		SetRelativeRotation(glm::inverse(_parent->GetWorldRotation()) * worldRotation);
		return;
	}
	SetRelativeRotation(worldRotation);
}

void UTransform::SetWorldScale(const glm::vec3& worldScale)
{
	if (_parent)
	{
		SetRelativeScale(worldScale / _parent->GetWorldScale());
		return;
	}
	SetRelativeScale(worldScale);
}

void UTransform::AddPosition(const glm::vec3& position)
{
	SetRelativePosition(_relativePosition + position);
}

void UTransform::AddRotation(const glm::quat& rotation)
{
	SetRelativeRotation(rotation * _relativeRotation);
}

void UTransform::AddScale(const glm::vec3& scale)
{
	SetRelativeScale(_relativeScale * scale);
}

void UTransform::SetParent(UTransform* parent, const ETransformSpace keepTransform)
{
	switch (keepTransform)
	{
	case ETransformSpace::Relative:
	{
		_parent = parent;
		break;
	}
	case ETransformSpace::World:
	{
		const UTransform clone = *this;
		_parent = parent;
		SetWorldPosition(clone.GetWorldPosition());
		SetWorldRotation(clone.GetWorldRotation());
		SetWorldScale(clone.GetWorldScale());
		break;
	}
	default:
		break;
	}
}

const glm::mat4 UTransform::GetTranslationMatrix() const
{
	return glm::translate(glm::identity<glm::mat4>(), GetWorldPosition());
}

const glm::mat4 UTransform::GetRotationMatrix() const
{
	return glm::mat4_cast(GetWorldRotation());
}

const glm::mat4 UTransform::GetScaleMatrix() const
{
	return glm::scale(glm::identity<glm::mat4>(), GetWorldScale());
}

const glm::mat4 UTransform::GetModelMatrix() const
{
	return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
}

const glm::vec3 UTransform::GetScreenPosition() const
{
	// cannot use camera in .h, circular include
	throw std::logic_error("not implemented");
}

const glm::vec3 UTransform::GetDirection(UTransform* target) const
{
	return target->GetWorldPosition() - GetWorldPosition();
}

const float UTransform::GetDistance(UTransform* other) const
{
	return glm::distance(GetWorldPosition(), other->GetWorldPosition());
}

