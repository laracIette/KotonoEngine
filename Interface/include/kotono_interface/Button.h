#pragma once
#include "generated/Button.generated.h"
#include <kotono_core/Widget.h>

#include <kotono_graphics/Color.h>
/// Set the widget's bounds as pressable
class WButton final : public WWidget
{
	GENERATED_WBUTTON()

public:
	WButton();

	b8 OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position) override;
	b8 OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) override;

	void OnUnfocused() override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

private:
	ReadonlyProperty(b8, isPressed_, IsPressed);
	ReadonlyProperty(b8, isSelected_, IsSelected);
	WritableProperty(b8, isEnabled_, IsEnabled);
	WritableProperty(b8, isSelectable_, IsSelectable);
	WritableProperty(VoidCallback, onClicked_, OnClicked);
	WritableProperty(VoidCallback, onPressed_, OnPressed);
	WritableProperty(VoidCallback, onDown_, OnDown);
	WritableProperty(VoidCallback, onActive_, OnActive);
	WritableProperty(VoidCallback, onInactive_, OnInactive);
	WritableProperty(VoidCallback, onSelected_, OnSelected);
	WritableProperty(VoidCallback, onDeselected_, OnDeselected);
	WritableProperty(UColor, normalColor_, NormalColor);
	WritableProperty(UColor, focusedColor_, FocusedColor);
	WritableProperty(UColor, pressedColor_, PressedColor);
	WritableProperty(UColor, selectedColor_, SelectedColor);
	WritableProperty(UColor, disabledColor_, DisabledColor);
};

