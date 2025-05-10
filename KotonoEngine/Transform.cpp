#include "Transform.h"
#include <stdexcept>

UTransform::UTransform() :
	relativePosition_(0.0f, 0.0f, 0.0f),
	relativeRotation_(glm::identity<glm::quat>()),
	relativeScale_(1.0f, 1.0f, 1.0f),
	parent_(nullptr)
{
}

const glm::vec3& UTransform::GetRelativePosition() const
{
	return relativePosition_;
}

const glm::quat& UTransform::GetRelativeRotation() const
{
	return relativeRotation_;
}

const glm::vec3& UTransform::GetRelativeScale() const
{
	return relativeScale_;
}

const glm::vec3 UTransform::GetWorldPosition() const
{
	if (parent_)
	{
		return parent_->GetWorldPosition() + relativePosition_;
	}
	return relativePosition_;
}

const glm::quat UTransform::GetWorldRotation() const
{
	if (parent_)
	{
		return glm::normalize(parent_->GetWorldRotation() * relativeRotation_);
	}
	return relativeRotation_;
}

const glm::vec3 UTransform::GetWorldScale() const
{
	if (parent_)
	{
		return parent_->GetWorldScale() * relativeScale_;
	}
	return relativeScale_;
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
	return parent_;
}

void UTransform::SetRelativePosition(const glm::vec3& relativePosition)
{
	relativePosition_ = relativePosition;
}

void UTransform::SetRelativeRotation(const glm::quat& relativeRotation)
{
	relativeRotation_ = glm::normalize(relativeRotation);
}

void UTransform::SetRelativeScale(const glm::vec3& relativeScale)
{
	relativeScale_ = relativeScale;
}

void UTransform::SetWorldPosition(const glm::vec3& worldPosition)
{
	if (parent_)
	{
		SetRelativePosition(worldPosition - parent_->GetWorldPosition());
		return;
	}
	SetRelativePosition(worldPosition);
}

void UTransform::SetWorldRotation(const glm::quat& worldRotation)
{
	if (parent_)
	{
		SetRelativeRotation(glm::inverse(parent_->GetWorldRotation()) * worldRotation);
		return;
	}
	SetRelativeRotation(worldRotation);
}

void UTransform::SetWorldScale(const glm::vec3& worldScale)
{
	if (parent_)
	{
		SetRelativeScale(worldScale / parent_->GetWorldScale());
		return;
	}
	SetRelativeScale(worldScale);
}

void UTransform::AddOffset(const glm::vec3& offset)
{
	SetRelativePosition(relativePosition_ + offset);
}

void UTransform::AddRotation(const glm::quat& rotation)
{
	SetRelativeRotation(rotation * relativeRotation_);
}

void UTransform::AddScale(const glm::vec3& scale)
{
	SetRelativeScale(relativeScale_ * scale);
}

void UTransform::SetParent(UTransform* parent, const ECoordinateSpace keepTransform)
{
	switch (keepTransform)
	{
	case ECoordinateSpace::Relative:
	{
		parent_ = parent;
		break;
	}
	case ECoordinateSpace::World:
	{
		const UTransform clone = *this;
		parent_ = parent;
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

const glm::vec3 UTransform::GetDirection(const UTransform& target) const
{
	return target.GetWorldPosition() - GetWorldPosition();
}

const float UTransform::GetDistance(const UTransform& other) const
{
	return glm::distance(GetWorldPosition(), other.GetWorldPosition());
}

