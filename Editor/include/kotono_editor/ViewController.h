#pragma once
#include "generated/ViewController.generated.h"
#include <kotono_core/SceneWidget.h>
class WSceneTexture;
class WViewController final : public WSceneWidget
{
	GENERATED_WVIEWCONTROLLER()

public:
	WViewController();
	WViewController(UScene* scene);

protected:
	WidgetPtr Build() override;

public:
	b8 OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position) override;
	b8 OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) override;
	b8 OnMouseScroll(glm::vec2 const& delta) override;

	b8 OnKeyboardKey(EKey key, EInputState inputState) override;

	void OnUnfocused() override;

private:
	void Translate(glm::vec3 const& delta) const;

private:
	b8 isActive_;

	f32 speed_;
	f32 sensitivity_;
	f32 pitch_;
	f32 yaw_;

	UPtr<WSceneTexture> sceneTexture_;
};