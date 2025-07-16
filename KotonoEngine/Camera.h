#pragma once
#include "SceneObject.h"
class TCamera final : public TSceneObject
{
	BASECLASS(TSceneObject)

public:
	void Init() override;
	void Cleanup() override;

	void Use();

private:
	bool isCurrent_;

	// Vertical fov in degrees
	float fov_;
	float depthNear_;
	float depthFar_;
	float speed_;
	float sensitivity_;
	float pitch_;
	float yaw_;

	void OnKeyboardWKeyDown() const;
	void OnKeyboardAKeyDown() const;
	void OnKeyboardSKeyDown() const;
	void OnKeyboardDKeyDown() const;
	void OnMouseRightButtonDown();

	void OnEventUpdateTransform() const;
};

