#include "Camera.h"
#include <algorithm>
#include <kotono_common/log.h>
#include <kotono_core/TimeManager.h>
#include <kotono_graphics/PipelineResourceManager.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#include <kotono_platform/glm_utils.h>
#include <kotono_platform/WindowViewport.h>

void SCamera::Init()
{
	fov_ = 90.0f;
	depthNear_ = 0.01f;
	depthFar_ = 1000.0f;
	speed_ = 1.0f;
	sensitivity_ = 0.005f;

	eventTransformUpdated_.AddListener(this, &SCamera::OnEventUpdateTransform);
	SetPosition(WorldForwardVector * -3.0f + WorldUpVector);

	Keyboard.EventKey(EKey::W, EInputState::Down).AddListener(this, &SCamera::OnKeyboardWKeyDown);
	Keyboard.EventKey(EKey::A, EInputState::Down).AddListener(this, &SCamera::OnKeyboardAKeyDown);
	Keyboard.EventKey(EKey::S, EInputState::Down).AddListener(this, &SCamera::OnKeyboardSKeyDown);
	Keyboard.EventKey(EKey::D, EInputState::Down).AddListener(this, &SCamera::OnKeyboardDKeyDown);
	Keyboard.EventKey(EKey::Q, EInputState::Down).AddListener(this, &SCamera::OnKeyboardQKeyDown);
	Keyboard.EventKey(EKey::E, EInputState::Down).AddListener(this, &SCamera::OnKeyboardEKeyDown);
	Mouse.EventButton(EButton::Right, EInputState::Pressed).AddListener(this, &SCamera::OnMouseRightButtonPressed);
	Mouse.EventButton(EButton::Right, EInputState::Released).AddListener(this, &SCamera::OnMouseRightButtonReleased);
	Mouse.EventMove().AddListener(this, &SCamera::OnMouseMove);
	Mouse.EventVerticalScroll().AddListener(this, &SCamera::OnMouseVerticalScroll);
}

void SCamera::Cleanup()
{
	Keyboard.EventKey(EKey::W, EInputState::Down).RemoveListener(this, &SCamera::OnKeyboardWKeyDown);
	Keyboard.EventKey(EKey::A, EInputState::Down).RemoveListener(this, &SCamera::OnKeyboardAKeyDown);
	Keyboard.EventKey(EKey::S, EInputState::Down).RemoveListener(this, &SCamera::OnKeyboardSKeyDown);
	Keyboard.EventKey(EKey::D, EInputState::Down).RemoveListener(this, &SCamera::OnKeyboardDKeyDown);
	Keyboard.EventKey(EKey::Q, EInputState::Down).RemoveListener(this, &SCamera::OnKeyboardQKeyDown);
	Keyboard.EventKey(EKey::E, EInputState::Down).RemoveListener(this, &SCamera::OnKeyboardEKeyDown);
	Mouse.EventButton(EButton::Right, EInputState::Pressed).RemoveListener(this, &SCamera::OnMouseRightButtonPressed);
	Mouse.EventButton(EButton::Right, EInputState::Released).RemoveListener(this, &SCamera::OnMouseRightButtonReleased);
	Mouse.EventMove().RemoveListener(this, &SCamera::OnMouseMove);
	Mouse.EventVerticalScroll().RemoveListener(this, &SCamera::OnMouseVerticalScroll);
}

glm::vec3 SCamera::RightVector() const
{
	return transform_.rotation * WorldRightVector;
}

glm::vec3 SCamera::UpVector() const
{
	return transform_.rotation * WorldUpVector;
}

glm::vec3 SCamera::ForwardVector() const
{
	return transform_.rotation * WorldForwardVector;
}

void SCamera::SetPosition(const glm::vec3& position)
{
	transform_.position = position;
	eventTransformUpdated_.Broadcast();
}

void SCamera::SetRotation(const glm::quat& rotation)
{
	transform_.rotation = rotation;
	eventTransformUpdated_.Broadcast();
}

void SCamera::Translate(const glm::vec3& offset)
{
	if (!isFocused_)
	{
		return;
	}
	SetPosition(transform_.position + offset);
}

void SCamera::OnKeyboardWKeyDown()
{
	const auto direction{ ForwardVector() };
	const auto delta{ direction * TimeManager.Delta() };
	Translate(delta * speed_);
}

void SCamera::OnKeyboardAKeyDown()
{
	const auto direction{ RightVector() };
	const auto delta{ direction * TimeManager.Delta() };
	Translate(delta * speed_);
}

void SCamera::OnKeyboardSKeyDown()
{
	const auto direction{ -ForwardVector() };
	const auto delta{ direction * TimeManager.Delta() };
	Translate(delta * speed_);
}

void SCamera::OnKeyboardDKeyDown()
{
	const auto direction{ -RightVector() };
	const auto delta{ direction * TimeManager.Delta() };
	Translate(delta * speed_);
}

void SCamera::OnKeyboardQKeyDown()
{
	Translate(-WorldUpVector * TimeManager.Delta() * speed_);
}

void SCamera::OnKeyboardEKeyDown()
{
	Translate(WorldUpVector * TimeManager.Delta() * speed_);
}

void SCamera::OnMouseRightButtonPressed()
{
	isFocused_ = true;
}

void SCamera::OnMouseRightButtonReleased()
{
	isFocused_ = false;
}

void SCamera::OnMouseMove(const glm::vec2 delta)
{
	if (!isFocused_)
	{
		return;
	}

	pitch_ -= delta.y * sensitivity_;
	yaw_ += delta.x * sensitivity_;

	// Clamp pitch to avoid flipping
	pitch_ = glm::clamp(pitch_, -glm::half_pi<float>(), glm::half_pi<float>());

	const glm::quat qPitch{ glm::angleAxis(pitch_, WorldRightVector) };
	const glm::quat qYaw{ glm::angleAxis(yaw_, WorldUpVector) };
	
	const glm::quat rotation{ qYaw * qPitch };
	SetRotation(rotation);
}

void SCamera::OnMouseVerticalScroll(const float delta)
{
	if (!isFocused_)
	{
		return;
	}

	speed_ += speed_ * delta / 10.0f;
	speed_ = std::clamp(speed_, 0.1f, 100.0f);
}

void SCamera::OnEventUpdateTransform() const
{
	const auto view{ glm::lookAt(transform_.position, transform_.position + ForwardVector(), UpVector()) };
	auto proj{ glm::perspective(glm::radians(fov_), WindowViewport.GetAspectRatio(), depthNear_, depthFar_) };
	proj[1][1] *= -1.0f;

	//Renderer.SceneRenderer().SetUniformData(ubo);
	PipelineResourceManager.SetFrameUBO({
		.view = view,
		.proj = proj,
		.viewProj = proj * view,
		.viewPos = glm::vec4{ transform_.position, 1.0f },
		.time = TimeManager.Now(),
	});
}
