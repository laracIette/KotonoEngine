#include "ViewController.h"

#include <glm/ext/quaternion_trigonometric.hpp>
#include <kotono_core/Interface.h>
#include <kotono_core/Scene.h>
#include <kotono_math/math_utils.h>

WViewController::WViewController()
	: WViewController(nullptr)
{
}

WViewController::WViewController(UScene* scene)
	: Base(scene)
	, isActive_{ false }
	, speed_{ 1.0f }
	, sensitivity_{ 0.005f }
	, pitch_{ 0.0f }
	, yaw_{ glm::radians(180.0f) }
	, position_{ WorldUpVector + WorldForwardVector * 2.0f }
{
}

b8 WViewController::OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position)
{
	if (button != EButton::Left)
	{
		return INPUT_UNHANDLED;
	}

	switch (inputState)
	{
	case EInputState::Pressed:
	{
		isActive_ = true;
		return INPUT_HANDLED;
	}
	case EInputState::Released:
	{
		isActive_ = false;
		return INPUT_HANDLED;
	}
	default: 
		break;
	}

	return INPUT_UNHANDLED;
}

b8 WViewController::OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position)
{
	if (!isActive_)
	{
		return INPUT_UNHANDLED;
	}

	pitch_ -= delta.y * sensitivity_;
	yaw_ += delta.x * sensitivity_;

	// Clamp pitch to avoid flipping
	pitch_ = glm::clamp(pitch_, -glm::half_pi<f32>(), glm::half_pi<f32>());

	glm::quat const qPitch{ glm::angleAxis(pitch_, WorldRightVector) };
	glm::quat const qYaw{ glm::angleAxis(yaw_, WorldUpVector) };

	if (onLook_)
	{
		rotation_ = qYaw * qPitch;
		onLook_(rotation_);
	}

	return INPUT_HANDLED;
}

b8 WViewController::OnMouseScroll(glm::vec2 const& delta)
{
	if (!isActive_)
	{
		return INPUT_UNHANDLED;
	}
	
	speed_ += speed_ * delta.y / 10.0f;
	speed_ = std::clamp(speed_, 0.1f, 100.0f);

	return INPUT_HANDLED;
}

b8 WViewController::OnKeyboardKey(EKey key, EInputState inputState)
{
	if (!isActive_)
	{
		return INPUT_UNHANDLED;
	}

	if (inputState != EInputState::Down)
	{
		return INPUT_UNHANDLED;
	}

	switch (key)
	{
	case EKey::W:
	{
		Translate(forward_vector(rotation_));
		return INPUT_HANDLED;
	}
	case EKey::A:
	{
		Translate(right_vector(rotation_));
		return INPUT_HANDLED;
	}
	case EKey::S:
	{
		Translate(-forward_vector(rotation_));
		return INPUT_HANDLED;
	}
	case EKey::D:
	{
		Translate(-right_vector(rotation_));
		return INPUT_HANDLED;
	}
	case EKey::Q:
	{
		Translate(-WorldUpVector);
		return INPUT_HANDLED;
	}
	case EKey::E:
	{
		Translate(WorldUpVector);
		return INPUT_HANDLED;
	}
	default:
		break;
	}

	return INPUT_UNHANDLED;
}

void WViewController::OnUnfocused()
{
	isActive_ = false;
}

void WViewController::Translate(glm::vec3 const& delta)
{
	if (onMove_)
	{
		position_ += delta * GetInterface()->GetDeltaTime() * speed_;
		onMove_(position_);
	}
}

#include "generated/ViewController.generated.inl"