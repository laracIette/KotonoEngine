#include "Camera.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/WindowViewport.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/Clock.h>
#include "SceneComponent.h"
#include "Engine.h"
#include "TimeManager.h"
#include "log.h"

void TCamera::Init()
{
	Base::Init();

	fov_ = 90.0f;
	depthNear_ = 0.01f;
	depthFar_ = 1000.0f;
	speed_ = 1.0f;
	sensitivity_ = 0.005f;

	RootComponent()->EventTransformUpdated().AddListener(KtDelegate<>(this, &TCamera::OnEventUpdateTransform));
	RootComponent()->SetWorldPosition(glm::vec3(0.0f, 0.0f, -3.0f));

	// ISSUE: input manager is engine time, not game time

	Framework.InputManager().Keyboard().KeyEvent(KT_KEY_W, KT_INPUT_STATE_DOWN).AddListener(KtDelegate(this, &TCamera::OnKeyboardWKeyDown));
	Framework.InputManager().Keyboard().KeyEvent(KT_KEY_A, KT_INPUT_STATE_DOWN).AddListener(KtDelegate(this, &TCamera::OnKeyboardAKeyDown));
	Framework.InputManager().Keyboard().KeyEvent(KT_KEY_S, KT_INPUT_STATE_DOWN).AddListener(KtDelegate(this, &TCamera::OnKeyboardSKeyDown));
	Framework.InputManager().Keyboard().KeyEvent(KT_KEY_D, KT_INPUT_STATE_DOWN).AddListener(KtDelegate(this, &TCamera::OnKeyboardDKeyDown));
	Framework.InputManager().Mouse().ButtonEvent(KT_BUTTON_RIGHT, KT_INPUT_STATE_PRESSED).AddListener(KtDelegate(this, &TCamera::OnMouseRightButtonPressed));
	Framework.InputManager().Mouse().ButtonEvent(KT_BUTTON_RIGHT, KT_INPUT_STATE_RELEASED).AddListener(KtDelegate(this, &TCamera::OnMouseRightButtonReleased));
	Framework.InputManager().Mouse().EventMove().AddListener(KtDelegate(this, &TCamera::OnMouseMove));
	Framework.InputManager().Mouse().EventVerticalScroll().AddListener(KtDelegate(this, &TCamera::OnMouseVerticalScroll));
}

void TCamera::Cleanup()
{
	Base::Cleanup();

	Framework.InputManager().Keyboard().KeyEvent(KT_KEY_W, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate(this, &TCamera::OnKeyboardWKeyDown));
	Framework.InputManager().Keyboard().KeyEvent(KT_KEY_A, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate(this, &TCamera::OnKeyboardAKeyDown));
	Framework.InputManager().Keyboard().KeyEvent(KT_KEY_S, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate(this, &TCamera::OnKeyboardSKeyDown));
	Framework.InputManager().Keyboard().KeyEvent(KT_KEY_D, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate(this, &TCamera::OnKeyboardDKeyDown));
	Framework.InputManager().Mouse().ButtonEvent(KT_BUTTON_RIGHT, KT_INPUT_STATE_PRESSED).RemoveListener(KtDelegate(this, &TCamera::OnMouseRightButtonPressed));
	Framework.InputManager().Mouse().ButtonEvent(KT_BUTTON_RIGHT, KT_INPUT_STATE_RELEASED).RemoveListener(KtDelegate(this, &TCamera::OnMouseRightButtonReleased));
	Framework.InputManager().Mouse().EventMove().RemoveListener(KtDelegate(this, &TCamera::OnMouseMove));
	Framework.InputManager().Mouse().EventVerticalScroll().RemoveListener(KtDelegate(this, &TCamera::OnMouseVerticalScroll));
}

void TCamera::Use()
{
	isCurrent_ = true;
}

void TCamera::OnKeyboardWKeyDown() const
{
	const auto direction = RootComponent()->ForwardVector();
	const auto delta = direction * Engine.TimeManager().EditorDelta();
	RootComponent()->Translate(delta * speed_);
}

void TCamera::OnKeyboardAKeyDown() const
{
	const auto direction = -RootComponent()->RightVector();
	const auto delta = direction * Engine.TimeManager().EditorDelta();
	RootComponent()->Translate(delta * speed_);
}

void TCamera::OnKeyboardSKeyDown() const
{
	const auto direction = -RootComponent()->ForwardVector();
	const auto delta = direction * Engine.TimeManager().EditorDelta();
	RootComponent()->Translate(delta * speed_);
}

void TCamera::OnKeyboardDKeyDown() const
{
	const auto direction = RootComponent()->RightVector();
	const auto delta = direction * Engine.TimeManager().EditorDelta();
	RootComponent()->Translate(delta * speed_);
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

	RootComponent()->SetWorldRotation(rotation);
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
	ubo.View = glm::lookAt(RootComponent()->GetWorldPosition(), RootComponent()->GetWorldPosition() + RootComponent()->ForwardVector(), RootComponent()->UpVector());
	ubo.Projection = glm::perspective(glm::radians(fov_), GetViewport()->GetAspectRatio(), depthNear_, depthFar_);
	//ubo.Projection[1][1] *= -1.0f;

	Framework.Renderer().GetSceneRenderer().SetUniformData(ubo);
}
