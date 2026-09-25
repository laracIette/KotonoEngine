#pragma once
#include "generated/ViewController.generated.h"
#include <kotono_core/SceneWidget.h>

#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>

class WViewController final : public WSceneWidget
{
	GENERATED_WVIEWCONTROLLER()

private:
	using LookCallback = std::function<void(glm::quat)>;
	using MoveCallback = std::function<void(glm::vec3)>;

public:
	WViewController();
	WViewController(UScene* scene);

	auto OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position) -> b8 override;
	auto OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) -> b8 override;
	auto OnMouseScroll(glm::vec2 const& delta) -> b8 override;

	auto OnKeyboardKey(EKey key, EInputState inputState) -> b8 override;

	void OnUnfocused() override;

private:
	void Translate(glm::vec3 const& delta);

private:
	b8 isActive_;

	f32 speed_;
	f32 sensitivity_;

	f32 pitch_;
	f32 yaw_;

	glm::vec3 position_;
	glm::quat rotation_;

	WritableProperty(LookCallback, onLook_, OnLook);
	WritableProperty(MoveCallback, onMove_, OnMove);
};