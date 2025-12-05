#include "Camera.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_platform/WindowViewport.h>
#include <kotono_input/InputManager.h>
#include <kotono_timing/TimeManager.h>
#include <kotono_engine/Engine.h>
#include <kotono_engine/TimeManager.h>
#include <algorithm>
#include "log.h"

void SCamera::Init()
{
	Use();

	fov_ = 90.0f;
	depthNear_ = 0.01f;
	depthFar_ = 1000.0f;
	speed_ = 1.0f;
	sensitivity_ = 0.005f;

	eventTransformUpdated_.AddListener(KtDelegate(this, &SCamera::OnEventUpdateTransform));
	SetPosition({ 0.0f, 0.0f, -3.0f });

	// todo: input manager is engine time, not game time

	InputManager.Keyboard().EventKey(KT_KEY_W, KT_INPUT_STATE_DOWN).AddListener(KtDelegate(this, &SCamera::OnKeyboardWKeyDown));
	InputManager.Keyboard().EventKey(KT_KEY_A, KT_INPUT_STATE_DOWN).AddListener(KtDelegate(this, &SCamera::OnKeyboardAKeyDown));
	InputManager.Keyboard().EventKey(KT_KEY_S, KT_INPUT_STATE_DOWN).AddListener(KtDelegate(this, &SCamera::OnKeyboardSKeyDown));
	InputManager.Keyboard().EventKey(KT_KEY_D, KT_INPUT_STATE_DOWN).AddListener(KtDelegate(this, &SCamera::OnKeyboardDKeyDown));
	InputManager.Mouse().EventButton(KT_BUTTON_RIGHT, KT_INPUT_STATE_PRESSED).AddListener(KtDelegate(this, &SCamera::OnMouseRightButtonPressed));
	InputManager.Mouse().EventButton(KT_BUTTON_RIGHT, KT_INPUT_STATE_RELEASED).AddListener(KtDelegate(this, &SCamera::OnMouseRightButtonReleased));
	InputManager.Mouse().EventMove().AddListener(KtDelegate(this, &SCamera::OnMouseMove));
	InputManager.Mouse().EventVerticalScroll().AddListener(KtDelegate(this, &SCamera::OnMouseVerticalScroll));
}

void SCamera::Cleanup()
{
	InputManager.Keyboard().EventKey(KT_KEY_W, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate(this, &SCamera::OnKeyboardWKeyDown));
	InputManager.Keyboard().EventKey(KT_KEY_A, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate(this, &SCamera::OnKeyboardAKeyDown));
	InputManager.Keyboard().EventKey(KT_KEY_S, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate(this, &SCamera::OnKeyboardSKeyDown));
	InputManager.Keyboard().EventKey(KT_KEY_D, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate(this, &SCamera::OnKeyboardDKeyDown));
	InputManager.Mouse().EventButton(KT_BUTTON_RIGHT, KT_INPUT_STATE_PRESSED).RemoveListener(KtDelegate(this, &SCamera::OnMouseRightButtonPressed));
	InputManager.Mouse().EventButton(KT_BUTTON_RIGHT, KT_INPUT_STATE_RELEASED).RemoveListener(KtDelegate(this, &SCamera::OnMouseRightButtonReleased));
	InputManager.Mouse().EventMove().RemoveListener(KtDelegate(this, &SCamera::OnMouseMove));
	InputManager.Mouse().EventVerticalScroll().RemoveListener(KtDelegate(this, &SCamera::OnMouseVerticalScroll));
}

void SCamera::Use()
{
	isCurrent_ = true;
}

glm::vec3 SCamera::RightVector() const
{
	return transform_.rotation * glm::vec3(-1.0f, 0.0f, 0.0f);
}

glm::vec3 SCamera::ForwardVector() const
{
	return transform_.rotation * glm::vec3(0.0f, 0.0f, 1.0f);
}

glm::vec3 SCamera::UpVector() const
{
	return transform_.rotation * glm::vec3(0.0f, -1.0f, 0.0f);
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
	SetPosition(transform_.position + offset);
}

void SCamera::Rotate(const glm::quat& rotation)
{
	SetRotation(rotation * transform_.rotation);
}

void SCamera::OnKeyboardWKeyDown()
{
	const auto direction{ ForwardVector() };
	const auto delta{ direction * Framework.TimeManager().Delta() };
	Translate(delta * speed_);
}

void SCamera::OnKeyboardAKeyDown()
{
	const auto direction{ -RightVector() };
	const auto delta{ direction * Framework.TimeManager().Delta() };
	Translate(delta * speed_);
}

void SCamera::OnKeyboardSKeyDown()
{
	const auto direction{ -ForwardVector() };
	const auto delta{ direction * Framework.TimeManager().Delta() };
	Translate(delta * speed_);
}

void SCamera::OnKeyboardDKeyDown()
{
	const auto direction{ RightVector() };
	const auto delta{ direction * Framework.TimeManager().Delta() };
	Translate(delta * speed_);
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

	pitch_ += delta.y * sensitivity_;
	yaw_ += delta.x * sensitivity_;

	// Clamp pitch to avoid flipping
	pitch_ = glm::clamp(pitch_, -glm::half_pi<float>(), glm::half_pi<float>());

	const glm::quat qPitch{ glm::angleAxis(pitch_, glm::vec3(1.0f, 0.0f, 0.0f)) };
	const glm::quat qYaw{ glm::angleAxis(yaw_, glm::vec3(0.0f, -1.0f, 0.0f)) };
	
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
	if (!isCurrent_)
	{
		return;
	}

	const KtSceneUniformData ubo{
		.view = glm::lookAt(transform_.position, transform_.position + ForwardVector(), UpVector()),
		.projection = glm::perspective(glm::radians(fov_), WindowViewport.GetAspectRatio(), depthNear_, depthFar_),
	};
	//ubo.projection[1][1] *= -1.0f;

	Framework.Renderer().GetSceneRenderer().SetUniformData(ubo);
}
