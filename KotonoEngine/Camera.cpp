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
	speed_ = 3.0f;
	sensitivity_ = 0.002f;

	GetRootComponent()->GetEventUpdateTransform().AddListener(KtDelegate<>(this, &TCamera::OnEventUpdateTransform));
	GetRootComponent()->SetWorldPosition(glm::vec3(0.0f, 0.0f, -3.0f));

	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_W, KT_INPUT_STATE_DOWN).AddListener(KtDelegate<>(this, &TCamera::OnKeyboardWKeyDown));
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_A, KT_INPUT_STATE_DOWN).AddListener(KtDelegate<>(this, &TCamera::OnKeyboardAKeyDown));
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_S, KT_INPUT_STATE_DOWN).AddListener(KtDelegate<>(this, &TCamera::OnKeyboardSKeyDown));
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_D, KT_INPUT_STATE_DOWN).AddListener(KtDelegate<>(this, &TCamera::OnKeyboardDKeyDown));
	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_RIGHT, KT_INPUT_STATE_DOWN).AddListener(KtDelegate<>(this, &TCamera::OnMouseRightButtonDown));
}

void TCamera::Cleanup()
{
	Base::Cleanup();

	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_W, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate<>(this, &TCamera::OnKeyboardWKeyDown));
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_A, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate<>(this, &TCamera::OnKeyboardAKeyDown));
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_S, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate<>(this, &TCamera::OnKeyboardSKeyDown));
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_D, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate<>(this, &TCamera::OnKeyboardDKeyDown));
	Framework.GetInputManager().GetMouse().GetButtonEvent(KT_BUTTON_RIGHT, KT_INPUT_STATE_DOWN).RemoveListener(KtDelegate<>(this, &TCamera::OnMouseRightButtonDown));
}

void TCamera::Use()
{
	isCurrent_ = true;
}

void TCamera::OnKeyboardWKeyDown() const
{
	const auto direction = GetRootComponent()->GetForwardVector();
	const auto delta = direction * Engine.GetTime().GetDelta();
	GetRootComponent()->Translate(delta);
}

void TCamera::OnKeyboardAKeyDown() const
{
	const auto direction = -GetRootComponent()->GetRightVector();
	const auto delta = direction * Engine.GetTime().GetDelta();
	GetRootComponent()->Translate(delta);
}

void TCamera::OnKeyboardSKeyDown() const
{
	const auto direction = -GetRootComponent()->GetForwardVector();
	const auto delta = direction * Engine.GetTime().GetDelta();
	GetRootComponent()->Translate(delta);
}

void TCamera::OnKeyboardDKeyDown() const
{
	const auto direction = GetRootComponent()->GetRightVector();
	const auto delta = direction * Engine.GetTime().GetDelta();
	GetRootComponent()->Translate(delta);
}

void TCamera::OnMouseRightButtonDown()
{
	const auto cursorDelta = Framework.GetInputManager().GetMouse().GetCursorPositionDelta();
	if (cursorDelta == glm::vec2(0.0f, 0.0f))
	{
		return;
	}

	pitch_ += cursorDelta.y * sensitivity_;
	yaw_ += cursorDelta.x * sensitivity_;

	// Clamp pitch to avoid flipping
	pitch_ = glm::clamp(pitch_, -glm::half_pi<float>(), glm::half_pi<float>());

	const glm::quat qPitch = glm::angleAxis(pitch_, glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::quat qYaw = glm::angleAxis(yaw_, glm::vec3(0.0f, -1.0f, 0.0f));

	const glm::quat rotation = qYaw * qPitch;

	GetRootComponent()->SetRelativeRotation(rotation);
}

void TCamera::OnEventUpdateTransform() const
{
	if (!isCurrent_)
	{
		return;
	}

	KtUniformData3D ubo{};
	ubo.View = glm::lookAt(GetRootComponent()->GetWorldPosition(), GetRootComponent()->GetWorldPosition() + GetRootComponent()->GetForwardVector(), glm::vec3(0.0f, -1.0f, 0.0f));
	ubo.Projection = glm::perspective(glm::radians(fov_), GetViewport()->GetAspectRatio(), depthNear_, depthFar_);
	//ubo.Projection[1][1] *= -1.0f;

	Framework.GetRenderer().GetRenderer3D().SetUniformData(ubo);
}
