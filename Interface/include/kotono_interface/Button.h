#pragma once
#include "generated/Button.generated.h"
#include <kotono_object/Widget.h>
/// Set the widget's bounds as pressable
class WButton final : public WWidget
{
	GENERATED_WBUTTON()

public:
	WButton();
	~WButton() override;

	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

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
	WritableProperty(VoidCallback, onActive_, OnActive);
	WritableProperty(VoidCallback, onInactive_, OnInactive);

	bool isPressed_;
};

