#pragma once
#include "generated/Button.generated.h"
#include "Widget.h"
/// Set the widget's bounds as pressable
class WButton final : public WWidget
{
	GENERATED_WBUTTON()

public:
	WButton();
	~WButton() override;

public:
	/// Returns whether the mouse was over the button
	bool ReceiveMouseLeftButtonPressed();
	/// Returns whether the button was pressed
	bool ReceiveMouseLeftButtonReleased();

	void OnMouseLeftButtonPressedNoInteract();

private:
	void OnMouseLeftButtonDown();

private:
	WritableProperty(bool, isEnabled_, IsEnabled);
	WritableProperty(VoidCallback, onClicked_, OnClicked);
	WritableProperty(VoidCallback, onPressed_, OnPressed);
	WritableProperty(VoidCallback, onDown_, OnDown);
	WritableProperty(VoidCallback, onPressOut_, OnPressOut);

	bool isPressed_;
};

