#pragma once
#include <kotono_core/Transform.h>
#include <kotono_common/Event.h>
class SCamera final
{
	friend class SEditor;

private:
	void Init();
	void Cleanup();

public:
	glm::vec3 RightVector() const;
	glm::vec3 UpVector() const;
	glm::vec3 ForwardVector() const;

	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::quat& rotation);

	void Translate(const glm::vec3& offset);

private:
	UTransform transform_;

	bool isFocused_;

	// Vertical fov in degrees
	float fov_;
	float depthNear_;
	float depthFar_;
	float speed_;
	float sensitivity_;
	float pitch_;
	float yaw_;

	UEvent<> eventTransformUpdated_;

	void OnKeyboardWKeyDown();
	void OnKeyboardAKeyDown();
	void OnKeyboardSKeyDown();
	void OnKeyboardDKeyDown();
	void OnMouseRightButtonPressed();
	void OnMouseRightButtonReleased();
	void OnMouseMove(const glm::vec2 delta);
	void OnMouseVerticalScroll(const float delta);

	void OnEventUpdateTransform() const;
};

inline SCamera Camera;
