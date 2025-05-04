#include "Rect.h"
#include <kotono_framework/Viewport.h>
#include <kotono_framework/log.h>

URect::URect(): 
	relativePosition_(0.0f, 0.0f),
	relativeRotation_(0.0f),
	relativeScale_(1.0f, 1.0f),
	baseSize_(0.0f, 0.0f),
	anchor_(EAnchor::Center),
	parent_(nullptr)
{
}

const glm::vec2& URect::GetRelativePosition() const
{
	return relativePosition_;
}

const float URect::GetRelativeRotation(const ERotationUnit unit) const
{
	if (unit == ERotationUnit::Degrees)
	{
		return glm::degrees(relativeRotation_);
	}
	return relativeRotation_;
}

const glm::vec2& URect::GetRelativeScale() const
{
	return relativeScale_;
}

const glm::vec2 URect::GetWorldPosition() const
{
	if (parent_)
	{
		return relativePosition_ + parent_->GetWorldPosition();
	}
	return relativePosition_;
}

const glm::vec2 URect::GetWorldPositionWithAnchorOffset() const
{
	if (parent_)
	{
		return relativePosition_ + GetAnchorOffset() + parent_->GetWorldPositionWithAnchorOffset();
	}
	return relativePosition_ + GetAnchorOffset();
}

const glm::vec2 URect::GetAnchorRelativePosition() const
{
	return relativePosition_ - GetAnchorOffset();
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
	if (parent_)
	{
		return GetRelativeRotation(unit) + parent_->GetWorldRotation(unit);
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
	return glm::vec2(baseSize_) * GetWorldScale();
}

const EAnchor URect::GetAnchor() const
{
	return anchor_;
}

const glm::vec2 URect::GetWorldScale() const
{
	if (parent_)
	{
		return relativeScale_ * parent_->GetWorldScale();
	}
	return relativeScale_;
}

const glm::vec2& URect::GetBaseSize() const
{
	return baseSize_;
}

URect* URect::GetParent() const
{
	return parent_;
}

void URect::SetRelativePosition(const glm::vec2& relativePosition)
{
	relativePosition_ = relativePosition;
}

void URect::SetRelativeScale(const glm::vec2& relativeScale)
{
	relativeScale_ = relativeScale;
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

	relativePosition_ = rotated + GetAnchorRelativePosition();
	relativeRotation_ = relativeRotation;
}

void URect::SetWorldPosition(const glm::vec2& worldPosition)
{
	if (parent_)
	{
		SetRelativePosition(worldPosition - parent_->GetWorldPosition());
		return;
	}
	SetRelativePosition(worldPosition);
}

void URect::SetWorldScale(const glm::vec2& worldScale)
{
	if (parent_)
	{
		SetRelativeScale(worldScale / parent_->GetWorldScale());
		return;
	}
	SetRelativeScale(worldScale);
}

void URect::SetWorldRotation(const float worldRotation, const ERotationUnit unit)
{
	if (parent_)
	{
		SetRelativeRotation(worldRotation - parent_->GetWorldRotation(unit), unit);
		return;
	}
	SetRelativeRotation(worldRotation, unit);
}

void URect::AddOffset(const glm::vec2& offset)
{
	SetRelativePosition(relativePosition_ + offset);
}

void URect::AddScale(const glm::vec2& scale)
{
	SetRelativeScale(relativeScale_ * scale);
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
	const auto newScale = screenSize / glm::vec2(baseSize_);
	SetWorldScale(newScale);
}

void URect::SetAnchor(const EAnchor anchor)
{
	anchor_ = anchor;
}

void URect::SetParent(URect* parent, const ECoordinateSpace keepRect)
{
	switch (keepRect)
	{
	case ECoordinateSpace::Relative:
	{
		parent_ = parent;
		break;
	}
	case ECoordinateSpace::World:
	{
		const URect clone = *this;
		parent_ = parent;
		SetWorldPosition(clone.GetWorldPosition());
		SetWorldScale(clone.GetWorldScale());
		SetWorldRotation(clone.GetWorldRotation());
		break;
	}
	default:
		break;
	}
}

void URect::SetBaseSize(const glm::vec2& baseSize)
{
	baseSize_ = baseSize;
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
		(anchor_ & EAnchor::Left) == EAnchor::Left ? relativeScale_.x / 2.0f
		: (anchor_ & EAnchor::Right) == EAnchor::Right ? -relativeScale_.x / 2.0f
		: 0.0f,
		(anchor_ & EAnchor::Top) == EAnchor::Top ? relativeScale_.y / 2.0f
		: (anchor_ & EAnchor::Bottom) == EAnchor::Bottom ? -relativeScale_.y / 2.0f
		: 0.0f
	);
}
