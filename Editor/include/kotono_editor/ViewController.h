#pragma once
#include "generated/ViewController.generated.h"
#include <kotono_core/SceneWidget.h>
class WSceneTexture;
class WViewController final : public WSceneWidget
{
	GENERATED_WVIEWCONTROLLER()

public:
	WViewController();
	WViewController(UPtr<WSceneContext> const& sceneContext);

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

	b8 OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) override;
	b8 OnMouseScroll(glm::vec2 const& delta, glm::vec2 const& position) override;

private:
	void OnKeyboardWKeyDown() const;
	void OnKeyboardAKeyDown() const;
	void OnKeyboardSKeyDown() const;
	void OnKeyboardDKeyDown() const;
	void OnKeyboardQKeyDown() const;
	void OnKeyboardEKeyDown() const;

	void Translate(glm::vec3 const& delta) const;

private:
	b8 isActive_;

	f32 speed_;
	f32 sensitivity_;
	f32 pitch_;
	f32 yaw_;

	UPtr<WSceneTexture> sceneTexture_;
};