#include "Camera.h"
#include "Engine.h"
#include <kotono_framework/Framework.h>

void TCamera::Init()
{
	Base::Init();
	_fov = 45.0f;
	_depthNear = 0.01f;
	_depthFar = 1000.0f;
	_speed = 3.0f;

	GetTransform().SetWorldLocation(glm::vec3(0.0f, -1.0f, 1.0f));

	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_LEFT, KT_INPUT_STATE_DOWN).AddListener(
		this, &TCamera::Move, glm::vec3(-1.0f, 0.0f, 0.0f)
	);
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_RIGHT, KT_INPUT_STATE_DOWN).AddListener(
		this, &TCamera::Move, glm::vec3(1.0f, 0.0f, 0.0f)
	);
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_UP, KT_INPUT_STATE_DOWN).AddListener(
		this, &TCamera::Move, glm::vec3(0.0f, 1.0f, 0.0f)
	);
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_DOWN, KT_INPUT_STATE_DOWN).AddListener(
		this, &TCamera::Move, glm::vec3(0.0f, -1.0f, 0.0f)
	);
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_DOWN).AddListener(
		this, &TCamera::Move, glm::vec3(0.0f, 0.0f, 1.0f)
	);
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_LEFT_CONTROL, KT_INPUT_STATE_DOWN).AddListener(
		this, &TCamera::Move, glm::vec3(0.0f, 0.0f, -1.0f)
	);
}

void TCamera::Cleanup()
{
	Base::Cleanup();
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_LEFT, KT_INPUT_STATE_DOWN).RemoveListener(this);
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_RIGHT, KT_INPUT_STATE_DOWN).RemoveListener(this);
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_UP, KT_INPUT_STATE_DOWN).RemoveListener(this);
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_DOWN, KT_INPUT_STATE_DOWN).RemoveListener(this);
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_DOWN).RemoveListener(this);
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_LEFT_CONTROL, KT_INPUT_STATE_DOWN).RemoveListener(this);
}

void TCamera::Use()
{
	KtUniformData3D ubo{};
	ubo.View = glm::lookAt(GetTransform().GetWorldLocation(), GetTransform().GetWorldLocation() + GetTransform().GetForwardVector(), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.Projection = glm::perspective(glm::radians(_fov), GetViewport()->GetAspectRatio(), _depthNear, _depthFar);
	ubo.Projection[1][1] *= -1.0f;

	Framework.GetRenderer().GetRenderer3D().SetUniformData(ubo);
}

void TCamera::Move(const glm::vec3& direction)
{
	const glm::vec3 offset = Engine.GetTime().GetDelta() * _speed * direction;
	GetTransform().SetRelativeLocation(GetTransform().GetRelativeLocation() + offset);
}
