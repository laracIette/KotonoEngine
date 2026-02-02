#pragma once
#include "Widget.h"
#include <functional>
class WButton : public WWidget
{
public:
	struct ButtonSettings
	{
		/// Triggers when the mouse left button has been pressed and released while hovering this button
		/// default = {}
		std::function<void()> onClick{};
		/// Triggers when the mouse left button is pressed while the mouse cursor hovers this button
		/// default = {}
		std::function<void()> onPress{};
		/// default = {}
		std::function<void()> onDown{};
		/// Triggers when the mouse left button is pressed while the mouse cursor doesn't hover this button
		/// default = {}
		std::function<void()> onPressOut{};
	};

	/// Set the widget's bounds as pressable
	WButton(const ButtonSettings& buttonSettings);

	void Cleanup() override;

	/// Returns whether the mouse was over the button
	bool ReceiveMouseLeftButtonPressed();
	/// Returns whether the button was pressed
	bool ReceiveMouseLeftButtonReleased();

	void OnMouseLeftButtonPressedNoInteract();

private:
	ButtonSettings buttonSettings_;
	bool isPressed_;

	void OnMouseLeftButtonDown();
};

