#pragma once
#include <Widget/Widget.h>

#include <Color.h>

#include "Button.generated.h"

/// Set the widget's bounds as interactable
class WButton final : public WWidget
{
	GENERATED()

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
	
protected:
	void Init() override;
	
public:
	auto OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8 override;
	auto OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) -> b8 override;

	void OnUnfocused() override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;
	
	/// Activate if the button is activatable and deactivated
	void Activate();
	
	/// Deactivate if the button is activatable and activated
	void Deactivate();
	
	GetterAndSetter(b8, UBindable<b8>, isEnabled_, IsEnabled);

	Setter(UColor, normalState_.color, NormalColor);
	Setter(UColor, focusedState_.color, FocusedColor);
	Setter(UColor, pressedState_.color, PressedColor);
	Setter(UColor, activatedState_.color, ActivatedColor);
	Setter(UColor, disabledState_.color, DisabledColor);

	Setter(UPath, normalState_.texture, NormalTexture);
	Setter(UPath, focusedState_.texture, FocusedTexture);
	Setter(UPath, pressedState_.texture, PressedTexture);
	Setter(UPath, activatedState_.texture, ActivatedTexture);
	Setter(UPath, disabledState_.texture, DisabledTexture);

protected:
	auto GetCanCache() const -> b8 override;

private:
	UBindable<b8> isEnabled_;

	ReadonlyProperty(b8, isPressed_, IsPressed);
	ReadonlyProperty(b8, isActivated_, IsActivated);

	WritableProperty(b8, isActivatable_, IsActivatable);
	WritableProperty(b8, startActivated_, StartActivated);

	WritableProperty(VoidCallback, onClicked_, OnClicked);
	WritableProperty(VoidCallback, onPressed_, OnPressed);
	WritableProperty(VoidCallback, onReleased_, OnReleased);
	WritableProperty(VoidCallback, onActivated_, OnActivated);
	WritableProperty(VoidCallback, onDeactivated_, OnDeactivated);

	WritableProperty(DragCallback, onDrag_, OnDrag);

	ReadonlyProperty(State, normalState_, NormalState);
	ReadonlyProperty(State, focusedState_, FocusedState);
	ReadonlyProperty(State, pressedState_, PressedState);
	ReadonlyProperty(State, activatedState_, ActivatedState);
	ReadonlyProperty(State, disabledState_, DisabledState);
};

