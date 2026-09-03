#pragma once
#include "generated/ViewController.generated.h"
#include <kotono_core/Widget.h>
class WSceneRenderer;
class WViewController final : public WWidget
{
	GENERATED_WVIEWCONTROLLER()

public:
	WViewController();

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

	b8 OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) override;

private:
	void OnKeyboardWKeyDown() const;
	void OnKeyboardAKeyDown() const;
	void OnKeyboardSKeyDown() const;
	void OnKeyboardDKeyDown() const;
	void OnKeyboardQKeyDown() const;
	void OnKeyboardEKeyDown() const;
	void OnMouseVerticalScroll(f32 delta);

	void Translate(glm::vec3 const& delta) const;

private:
	b8 isActive_;

	f32 speed_;
	f32 sensitivity_;
	f32 pitch_;
	f32 yaw_;

	UPtr<WSceneRenderer> sceneRenderer_;
};