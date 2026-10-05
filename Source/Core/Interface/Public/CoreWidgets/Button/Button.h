#pragma once
#include "generated/Button.generated.h"
#include <Widget/Widget.h>

#include <Color.h>

/// Set the widget's bounds as interactable
class WButton final : public WWidget
{
	GENERATED_WBUTTON()

public:
	using DragCallback = std::function<void(glm::vec2)>;

public:
	struct State
	{
		UPath texture;
		UColor color;
	};

public:
	WButton();

	auto OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8 override;
	auto OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) -> b8 override;

	void OnUnfocused() override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

	Getter(b8, isEnabled_, IsEnabled, Value);
	Setter(UBindable<b8>, isEnabled_, IsEnabled);

	GetterAndSetter(UColor, normalState_.color, NormalColor, Value);
	GetterAndSetter(UColor, focusedState_.color, FocusedColor, Value);
	GetterAndSetter(UColor, pressedState_.color, PressedColor, Value);
	GetterAndSetter(UColor, activatedState_.color, ActivatedColor, Value);
	GetterAndSetter(UColor, disabledState_.color, DisabledColor, Value);

	GetterAndSetter(UPath, normalState_.texture, NormalTexture);
	GetterAndSetter(UPath, focusedState_.texture, FocusedTexture);
	GetterAndSetter(UPath, pressedState_.texture, PressedTexture);
	GetterAndSetter(UPath, activatedState_.texture, ActivatedTexture);
	GetterAndSetter(UPath, disabledState_.texture, DisabledTexture);

protected:
	auto GetCanCache() const -> b8 override;

private:
	UBindable<b8> isEnabled_;

	ReadonlyProperty(b8, isPressed_, IsPressed, Value);
	WritableProperty(b8, isActivated_, IsActivated, Value);

	WritableProperty(b8, isActivatable_, IsActivatable, Value);

	WritableProperty(VoidCallback, onClicked_, OnClicked);
	WritableProperty(VoidCallback, onPressed_, OnPressed);
	WritableProperty(VoidCallback, onReleased_, OnReleased);
	WritableProperty(VoidCallback, onActivated_, OnActivated);
	WritableProperty(VoidCallback, onDeactivated_, OnDeactivated);

	WritableProperty(DragCallback, onDrag_, OnDrag);

	State normalState_;
	State focusedState_;
	State pressedState_;
	State activatedState_;
	State disabledState_;
};

