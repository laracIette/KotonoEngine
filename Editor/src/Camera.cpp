#include "Camera.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <kotono_core/TimeManager.h>
#include <kotono_graphics/RenderContext.h>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#include <kotono_platform/glm_utils.h>

void GCamera::Init()
{
	speed_ = 1.0f;
	sensitivity_ = 0.005f;
	yaw_ = glm::radians(180.0f);

	Keyboard.EventKey(EKey::W, EInputState::Down).AddListener(this, &GCamera::OnKeyboardWKeyDown);
	Keyboard.EventKey(EKey::A, EInputState::Down).AddListener(this, &GCamera::OnKeyboardAKeyDown);
	Keyboard.EventKey(EKey::S, EInputState::Down).AddListener(this, &GCamera::OnKeyboardSKeyDown);
	Keyboard.EventKey(EKey::D, EInputState::Down).AddListener(this, &GCamera::OnKeyboardDKeyDown);
	Keyboard.EventKey(EKey::Q, EInputState::Down).AddListener(this, &GCamera::OnKeyboardQKeyDown);
	Keyboard.EventKey(EKey::E, EInputState::Down).AddListener(this, &GCamera::OnKeyboardEKeyDown);
	Mouse.EventButton(EButton::Right, EInputState::Pressed).AddListener(this, &GCamera::OnMouseRightButtonPressed);
	Mouse.EventButton(EButton::Right, EInputState::Released).AddListener(this, &GCamera::OnMouseRightButtonReleased);
	Mouse.EventMove().AddListener(this, &GCamera::OnMouseMove);
	Mouse.EventVerticalScroll().AddListener(this, &GCamera::OnMouseVerticalScroll);
}

void GCamera::Cleanup()
{
	Keyboard.EventKey(EKey::W, EInputState::Down).RemoveListener(this, &GCamera::OnKeyboardWKeyDown);
	Keyboard.EventKey(EKey::A, EInputState::Down).RemoveListener(this, &GCamera::OnKeyboardAKeyDown);
	Keyboard.EventKey(EKey::S, EInputState::Down).RemoveListener(this, &GCamera::OnKeyboardSKeyDown);
	Keyboard.EventKey(EKey::D, EInputState::Down).RemoveListener(this, &GCamera::OnKeyboardDKeyDown);
	Keyboard.EventKey(EKey::Q, EInputState::Down).RemoveListener(this, &GCamera::OnKeyboardQKeyDown);
	Keyboard.EventKey(EKey::E, EInputState::Down).RemoveListener(this, &GCamera::OnKeyboardEKeyDown);
	Mouse.EventButton(EButton::Right, EInputState::Pressed).RemoveListener(this, &GCamera::OnMouseRightButtonPressed);
	Mouse.EventButton(EButton::Right, EInputState::Released).RemoveListener(this, &GCamera::OnMouseRightButtonReleased);
	Mouse.EventMove().RemoveListener(this, &GCamera::OnMouseMove);
	Mouse.EventVerticalScroll().RemoveListener(this, &GCamera::OnMouseVerticalScroll);
}

void GCamera::OnKeyboardWKeyDown() const
{
	const auto direction{ RenderContext.GetViewPoint().GetForwardVector() };
	Translate(direction * TimeManager.Delta() * speed_);
}

void GCamera::OnKeyboardAKeyDown() const
{
	const auto direction{ RenderContext.GetViewPoint().GetRightVector() };
	Translate(direction * TimeManager.Delta() * speed_);
}

void GCamera::OnKeyboardSKeyDown() const
{
	const auto direction{ -RenderContext.GetViewPoint().GetForwardVector() };
	Translate(direction * TimeManager.Delta() * speed_);
}

void GCamera::OnKeyboardDKeyDown() const
{
	const auto direction{ -RenderContext.GetViewPoint().GetRightVector() };
	Translate(direction * TimeManager.Delta() * speed_);
}

void GCamera::OnKeyboardQKeyDown() const
{
	Translate(-WorldUpVector * TimeManager.Delta() * speed_);
}

void GCamera::OnKeyboardEKeyDown() const
{
	Translate(WorldUpVector * TimeManager.Delta() * speed_);
}

void GCamera::OnMouseRightButtonPressed()
{
	isFocused_ = true;
}

void GCamera::OnMouseRightButtonReleased()
{
	isFocused_ = false;
}

void GCamera::OnMouseMove(const glm::vec2& delta)
{
	if (!isFocused_)
	{
		return;
	}

	pitch_ -= delta.y * sensitivity_;
	yaw_ += delta.x * sensitivity_;

	// Clamp pitch to avoid flipping
	pitch_ = glm::clamp(pitch_, -glm::half_pi<f32>(), glm::half_pi<f32>());

	const glm::quat qPitch{ glm::angleAxis(pitch_, WorldRightVector) };
	const glm::quat qYaw{ glm::angleAxis(yaw_, WorldUpVector) };
	
	const glm::quat rotation{ qYaw * qPitch };
	RenderContext.GetViewPoint().SetRotation(rotation);
}

void GCamera::OnMouseVerticalScroll(const f32 delta)
{
	if (!isFocused_)
	{
		return;
	}

	speed_ += speed_ * delta / 10.0f;
	speed_ = std::clamp(speed_, 0.1f, 100.0f);
}

void GCamera::Translate(const glm::vec3& delta) const
{
	if (!isFocused_)
	{
		return;
	}

	const auto& position{ RenderContext.GetViewPoint().GetPosition() };
	RenderContext.GetViewPoint().SetPosition(position + delta);
}
