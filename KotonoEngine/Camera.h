#pragma once
#include "SceneObject.h"
class TCamera final : public TSceneObject
{
	BASECLASS(TSceneObject)

public:
	void Init() override;

	void Use();

private:
	// Vertical fov in degrees
	float _fov;
	float _depthNear;
	float _depthFar;
	float _speed;
	float _sensitivity;
	float _pitch;
	float _yaw;
};

