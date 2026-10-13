#include "ViewController.h"

#include <core_widgets.h>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <Interface/Interface.h>
#include <math_utils/math_utils.h>
#include <Scene/Scene.h>

#include "SceneComponent/SceneComponent.h"

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

WidgetPtr WViewController::Build()
{
	return (
		UCreate<WButton>{}()
		| Apply(&WButton::SetTint, Colors::Transparent)
		| Apply(&WButton::SetOnPressed, [this]() { isActive_ = true; })
		| Apply(&WButton::SetOnReleased, [this]() { isActive_ = false; })
		| Apply(&WButton::SetOnClicked, [this]() {
			if (UPtr const sceneComponent{ GetScene()->GetRayIntersectSceneComponent(position_, rotation_ * WorldUpVector) })
			{
				GetScene()->SelectObject(sceneComponent->GetOwner());
			}
		})
		| Apply(&WButton::SetOnDrag, [this](glm::vec2 const& delta) {
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
		})
	);
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

b8 WViewController::OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier)
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

void WViewController::Translate(glm::vec3 const& delta)
{
	if (onMove_)
	{
		position_ += delta * GetInterface()->GetDeltaTime() * speed_;
		onMove_(position_);
	}
}

#include "ViewController.generated.inl"