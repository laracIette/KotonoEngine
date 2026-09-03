#pragma once
#include "generated/Button.generated.h"
#include <kotono_core/Widget.h>
/// Set the widget's bounds as pressable
class WButton final : public WWidget
{
	GENERATED_WBUTTON()

public:
	WButton();

	b8 OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position) override;
	b8 OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) override;

	void OnUnfocused() override;

private:
	WritableProperty(b8, isEnabled_, IsEnabled);
	WritableProperty(VoidCallback, onClicked_, OnClicked);
	WritableProperty(VoidCallback, onPressed_, OnPressed);
	WritableProperty(VoidCallback, onDown_, OnDown);
	WritableProperty(VoidCallback, onPressOut_, OnPressOut);
	WritableProperty(VoidCallback, onActive_, OnActive);
	WritableProperty(VoidCallback, onInactive_, OnInactive);
	ReadonlyProperty(b8, isPressed_, IsPressed);
};

