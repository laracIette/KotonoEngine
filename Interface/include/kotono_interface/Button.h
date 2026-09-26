#pragma once
#include "generated/Button.generated.h"
#include <kotono_core/Widget.h>

#include <kotono_common/Bindable.h>
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

	auto GetIsEnabled() const -> b8;
	auto GetNormalColor() const -> UColor;

	void SetIsEnabled(UBindable<b8> const& isEnabled);
	void SetNormalColor(UBindable<UColor> const& color);

protected:
	auto GetCanCache() const -> b8 override;

private:
	UBindable<b8> isEnabled_;
	UBindable<UColor> normalColor_;

	ReadonlyProperty(b8, isPressed_, IsPressed, Value);
	ReadonlyProperty(b8, isSelected_, IsSelected, Value);
	WritableProperty(b8, isSelectable_, IsSelectable, Value);
	WritableProperty(VoidCallback, onClicked_, OnClicked);
	WritableProperty(VoidCallback, onDown_, OnDown);
	WritableProperty(VoidCallback, onActive_, OnActive);
	WritableProperty(VoidCallback, onInactive_, OnInactive);
	WritableProperty(VoidCallback, onSelected_, OnSelected);
	WritableProperty(VoidCallback, onDeselected_, OnDeselected);
	WritableProperty(UColor, focusedColor_, FocusedColor, Value);
	WritableProperty(UColor, pressedColor_, PressedColor, Value);
	WritableProperty(UColor, selectedColor_, SelectedColor, Value);
	WritableProperty(UColor, disabledColor_, DisabledColor, Value);
};

