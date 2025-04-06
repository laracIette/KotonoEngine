#include "Camera.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include "Engine.h"

void TCamera::Init()
{
	Base::Init();
	_fov = 90.0f;
	_depthNear = 0.01f;
	_depthFar = 1000.0f;
	_speed = 3.0f;
	_sensitivity = 0.1f;

	GetTransform().SetWorldPosition(glm::vec3(0.0f, -1.0f, 1.0f));
}

void TCamera::Use()
{
	KtUniformData3D ubo{};
	ubo.View = glm::lookAt(GetTransform().GetWorldPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	ubo.Projection = glm::perspective(glm::radians(_fov), GetViewport()->GetAspectRatio(), _depthNear, _depthFar);
    //ubo.Projection[1][1] *= -1.0f;

	Framework.GetRenderer().GetRenderer3D().SetUniformData(ubo);
}
