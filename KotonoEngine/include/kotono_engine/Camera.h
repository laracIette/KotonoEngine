#pragma once
#include "SceneObject.h"
#include <kotono_framework/glm_includes.h>
class TCamera final : public TSceneObject
{
	BASECLASS(TSceneObject)

protected:
	void Init() override;
	void Cleanup() override;

public:
	void Use();

private:
	bool isCurrent_;
	bool isFocused_;

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
	void OnMouseRightButtonPressed();
	void OnMouseRightButtonReleased();
	void OnMouseMove(const glm::vec2 delta);
	void OnMouseVerticalScroll(const float delta);

	void OnEventUpdateTransform() const;
};

