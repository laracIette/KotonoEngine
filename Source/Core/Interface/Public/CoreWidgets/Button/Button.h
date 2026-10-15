#pragma once
#include <Widget/Widget.h>

#include "ButtonStyle.h"

#include "Button.generated.h"

/// Set the widget's bounds as interactable
class WButton final : public WWidget
{
	GENERATED()

public:
	using DragCallback = std::function<void(glm::vec2, glm::vec2)>;
	
public:
	WButton();
	
protected:
	void Init() override;
	
public:
	auto OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8 override;
	auto OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) -> b8 override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph, UColor const& tint) const override;
	
	/// Activate if the button is deactivated and triggers the OnActivated event
	void Activate();
	
	/// Deactivate if the button is activated and triggers the OnDeactivated event
	void Deactivate();
	
	GetterAndSetter(b8, UBindable<b8>, isEnabled_, IsEnabled);
	
	Setter(UPtr<KButtonStyle::Class const>, StyleClass, StyleClass);

protected:
	auto GetCanCache() const -> b8 override;

private:
	UBindable<b8> isEnabled_;

	ReadonlyProperty(b8, isPressed_, IsPressed);
	WritableProperty(b8, isActivated_, IsActivated);

	WritableProperty(b8, isActivatable_, IsActivatable);
	WritableProperty(b8, startActivated_, StartActivated);

	WritableProperty(VoidCallback, onClicked_, OnClicked);
	WritableProperty(VoidCallback, onPressed_, OnPressed);
	WritableProperty(VoidCallback, onReleased_, OnReleased);
	WritableProperty(VoidCallback, onActivated_, OnActivated);
	WritableProperty(VoidCallback, onDeactivated_, OnDeactivated);

	WritableProperty(DragCallback, onDrag_, OnDrag);
	
	glm::vec2 cursorPosition_;
	glm::vec2 pressedCursorPosition_;
	
	KButtonStyle::State disabledState_;
	KButtonStyle::State normalState_;
	KButtonStyle::State focusedState_;
	KButtonStyle::State pressedState_;
	KButtonStyle::State activatedNormalState_;
	KButtonStyle::State activatedFocusedState_;
	KButtonStyle::State activatedPressedState_;
};

