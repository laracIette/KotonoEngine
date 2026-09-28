#pragma once
#include "generated/Button.generated.h"
#include <kotono_core/Widget.h>

#include <kotono_common/Bindable.h>
#include <kotono_graphics/Color.h>


/// Set the widget's bounds as interactable
class WButton final : public WWidget
{
	GENERATED_WBUTTON()

public:
	struct State
	{
		UPath texture;
		UColor color;
	};

public:
	WButton();

	auto OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position) -> b8 override;
	auto OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) -> b8 override;

	void OnUnfocused() override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

	auto GetIsEnabled() const -> b8 { return isEnabled_; }
	void SetIsEnabled(UBindable<b8> const& isEnabled) { isEnabled_ = isEnabled; }

	auto GetNormalColor() const -> UColor { return normalState_.color; }
	auto GetFocusedColor() const -> UColor { return focusedState_.color; }
	auto GetPressedColor() const -> UColor { return pressedState_.color; }
	auto GetActivatedColor() const -> UColor { return activatedState_.color; }
	auto GetSelectedColor() const -> UColor { return selectedState_.color; }
	auto GetDisabledColor() const -> UColor { return disabledState_.color; }

	auto GetNormalTexture() const -> UPath const& { return normalState_.texture; }
	auto GetFocusedTexture() const -> UPath const& { return focusedState_.texture; }
	auto GetPressedTexture() const -> UPath const& { return pressedState_.texture; }
	auto GetActivatedTexture() const -> UPath const& { return activatedState_.texture; }
	auto GetSelectedTexture() const -> UPath const& { return selectedState_.texture; }
	auto GetDisabledTexture() const -> UPath const& { return disabledState_.texture; }

	void SetNormalColor(UColor const& color) { normalState_.color = color; }
	void SetFocusedColor(UColor const& color) { focusedState_.color = color; }
	void SetPressedColor(UColor const& color) { pressedState_.color = color; }
	void SetActivatedColor(UColor const& color) { activatedState_.color = color; }
	void SetSelectedColor(UColor const& color) { selectedState_.color = color; }
	void SetDisabledColor(UColor const& color) { disabledState_.color = color; }

	void SetNormalTexture(UPath const& path) { normalState_.texture = path; }
	void SetFocusedTexture(UPath const& path) { focusedState_.texture = path; }
	void SetPressedTexture(UPath const& path) { pressedState_.texture = path; }
	void SetActivatedTexture(UPath const& path) { activatedState_.texture = path; }
	void SetSelectedTexture(UPath const& path) { selectedState_.texture = path; }
	void SetDisabledTexture(UPath const& path) { disabledState_.texture = path; }

protected:
	auto GetCanCache() const -> b8 override;

private:
	UBindable<b8> isEnabled_;

	ReadonlyProperty(b8, isPressed_, IsPressed, Value);
	WritableProperty(b8, isActivated_, IsActivated, Value);
	WritableProperty(b8, isSelected_, IsSelected, Value);

	WritableProperty(b8, isActivatable_, IsActivatable, Value);
	WritableProperty(b8, isSelectable_, IsSelectable, Value);

	WritableProperty(VoidCallback, onClicked_, OnClicked);
	WritableProperty(VoidCallback, onDown_, OnDown);
	WritableProperty(VoidCallback, onPressed_, OnPressed);
	WritableProperty(VoidCallback, onReleased_, OnReleased);
	WritableProperty(VoidCallback, onActivated_, OnActivated);
	WritableProperty(VoidCallback, onDeactivated_, OnDeactivated);
	WritableProperty(VoidCallback, onSelected_, OnSelected);
	WritableProperty(VoidCallback, onDeselected_, OnDeselected);

	State normalState_;
	State focusedState_;
	State pressedState_;
	State activatedState_;
	State selectedState_;
	State disabledState_;
};

