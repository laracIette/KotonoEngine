#pragma once
#include "Widget.h"
#include <functional>
class WButton : public WWidget
{
public:
	struct ButtonSettings
	{
		/// default = {}
		std::function<void()> onPress{};
		/// default = {}
		std::function<void()> onDown{};
		/// default = {}
		std::function<void()> onReleased{};
		// default = {}
		std::function<void(float)> onVerticalScroll{};
	};

	/// Set the widget's bounds as pressable
	WButton(const ButtonSettings& buttonSettings);

	void Cleanup() override;

private:
	ButtonSettings buttonSettings_;
	bool isPressed_;

	bool IsMouseHovering() const;

	void OnMouseLeftButtonPressed();
	void OnMouseLeftButtonDown();
	void OnMouseLeftButtonReleased();
	void OnMouseVerticalScroll(const float delta);
};

