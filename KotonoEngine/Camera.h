#pragma once
#include "SceneObject.h"
class TCamera final : public TSceneObject
{
	BASECLASS(TSceneObject)

public:
	void Init() override;
	void Cleanup() override;

	void Use();	

	void Move(const glm::vec3& direction);

private:
	// Vertical fov in degrees
	float _fov;
	float _depthNear;
	float _depthFar;
	float _speed;
};

