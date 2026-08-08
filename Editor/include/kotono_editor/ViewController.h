#pragma once
#include "generated/ViewController.generated.h"
#include <kotono_interface/Widget.h>
class WViewController final : public WWidget
{
	GENERATED_WVIEWCONTROLLER()

public:
	WViewController();

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

private:
	void OnKeyboardWKeyDown() const;
	void OnKeyboardAKeyDown() const;
	void OnKeyboardSKeyDown() const;
	void OnKeyboardDKeyDown() const;
	void OnKeyboardQKeyDown() const;
	void OnKeyboardEKeyDown() const;
	void OnMouseMove(const glm::vec2& delta);
	void OnMouseVerticalScroll(const f32 delta);

	void Translate(const glm::vec3& delta) const;

private:
	bool isFocused_;
	f32 speed_;
	f32 sensitivity_;
	f32 pitch_;
	f32 yaw_;
};