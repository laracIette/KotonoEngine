#include "Camera.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/Viewport.h>
#include <kotono_framework/InputManager.h>
#include "SceneComponent.h"
#include "Engine.h"
#include "Time.h"
#include "log.h"

void TCamera::Init()
{
	Base::Init();

	fov_ = 90.0f;
	depthNear_ = 0.01f;
	depthFar_ = 1000.0f;
	speed_ = 1.0f;
	sensitivity_ = 0.01f;

	GetRootComponent()->GetEventUpdateTransform().AddListener(KtDelegate<>(this, &TCamera::OnEventUpdateTransform));
	GetRootComponent()->SetWorldPosition(glm::vec3(0.0f, 0.0f, -3.0f));

	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_W, KT_INPUT_STATE_DOWN).AddListener(KtDelegate(this, &TCamera::OnKeyboardWKeyDown));
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_A, KT_INPUT_STATE_DOWN).AddListener(KtDelegate(this, &TCamera::OnKeyboardAKeyDown));
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_S, KT_INPUT_STATE_DOWN).AddListener(KtDelegate(this, &TCamera::OnKeyboardSKeyDown));
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_D, KT_INPUT_STATE_DOWN).AddListener(KtDelegate(this, &TCamera::OnKeyboardDKeyDown));
	Framework.GetInputManager().GetMouse().GetEvent(KT_BUTTON_RIGHT, KT_INPUT_STATE_PRESSED).AddListener(KtDelegate(this, &TCamera::OnMouseRightButtonPressed));
	Framework.GetInputManager().GetMouse().GetEvent(KT_BUTTON_RIGHT, KT_INPUT_STATE_RELEASED).AddListener(KtDelegate(this, &TCamera::OnMouseRightButtonReleased));
	Framework.GetInputManager().GetMouse().GetEventMove().AddListener(KtDelegate(this, &TCamera::OnMouseMove));
	Framework.GetInputManager().GetMouse().GetEventVerticalScroll().AddListener(KtDelegate(this, &TCamera::OnMouseVerticalScroll));
}

void TCamera::Cleanup()
{
	Base::Cleanup();

	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_W, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate(this, &TCamera::OnKeyboardWKeyDown));
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_A, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate(this, &TCamera::OnKeyboardAKeyDown));
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_S, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate(this, &TCamera::OnKeyboardSKeyDown));
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_D, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate(this, &TCamera::OnKeyboardDKeyDown));
	Framework.GetInputManager().GetMouse().GetEvent(KT_BUTTON_RIGHT, KT_INPUT_STATE_PRESSED).RemoveListener(KtDelegate(this, &TCamera::OnMouseRightButtonPressed));
	Framework.GetInputManager().GetMouse().GetEvent(KT_BUTTON_RIGHT, KT_INPUT_STATE_RELEASED).RemoveListener(KtDelegate(this, &TCamera::OnMouseRightButtonReleased));
	Framework.GetInputManager().GetMouse().GetEventMove().RemoveListener(KtDelegate(this, &TCamera::OnMouseMove));
	Framework.GetInputManager().GetMouse().GetEventVerticalScroll().RemoveListener(KtDelegate(this, &TCamera::OnMouseVerticalScroll));
}

void TCamera::Use()
{
	isCurrent_ = true;
}

void TCamera::OnKeyboardWKeyDown() const
{
	const auto direction = GetRootComponent()->GetForwardVector();
	const auto delta = direction * Engine.GetTime().GetDelta();
	GetRootComponent()->Translate(delta * speed_);
}

void TCamera::OnKeyboardAKeyDown() const
{
	const auto direction = -GetRootComponent()->GetRightVector();
	const auto delta = direction * Engine.GetTime().GetDelta();
	GetRootComponent()->Translate(delta * speed_);
}

void TCamera::OnKeyboardSKeyDown() const
{
	const auto direction = -GetRootComponent()->GetForwardVector();
	const auto delta = direction * Engine.GetTime().GetDelta();
	GetRootComponent()->Translate(delta * speed_);
}

void TCamera::OnKeyboardDKeyDown() const
{
	const auto direction = GetRootComponent()->GetRightVector();
	const auto delta = direction * Engine.GetTime().GetDelta();
	GetRootComponent()->Translate(delta * speed_);
}

void TCamera::OnMouseRightButtonPressed()
{
	isFocused_ = true;
}

void TCamera::OnMouseRightButtonReleased()
{
	isFocused_ = false;
}

void TCamera::OnMouseMove(const glm::vec2 delta)
{
	if (!isFocused_)
	{
		return;
	}

	pitch_ += delta.y * sensitivity_;
	yaw_ += delta.x * sensitivity_;

	// Clamp pitch to avoid flipping
	pitch_ = glm::clamp(pitch_, -glm::half_pi<float>(), glm::half_pi<float>());

	const glm::quat qPitch = glm::angleAxis(pitch_, glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::quat qYaw = glm::angleAxis(yaw_, glm::vec3(0.0f, -1.0f, 0.0f));

	const glm::quat rotation = qYaw * qPitch;

	GetRootComponent()->SetWorldRotation(rotation);
}

void TCamera::OnMouseVerticalScroll(const float delta)
{
	if (!isFocused_)
	{
		return;
	}

	speed_ += speed_ * delta / 10.0f;
	speed_ = std::clamp(speed_, 0.1f, 100.0f);
}

void TCamera::OnEventUpdateTransform() const
{
	if (!isCurrent_)
	{
		return;
	}

	KtUniformData3D ubo{};
	ubo.View = glm::lookAt(GetRootComponent()->GetWorldPosition(), GetRootComponent()->GetWorldPosition() + GetRootComponent()->GetForwardVector(), GetRootComponent()->GetUpVector());
	ubo.Projection = glm::perspective(glm::radians(fov_), GetViewport()->GetAspectRatio(), depthNear_, depthFar_);
	//ubo.Projection[1][1] *= -1.0f;

	Framework.GetRenderer().GetSceneRenderer().SetUniformData(ubo);
}
