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

public:
	const VoidCallback& GetOnClicked() const;
	const VoidCallback& GetOnPressed() const;
	const VoidCallback& GetOnDown() const;
	const VoidCallback& GetOnPressOut() const;

	void SetOnClicked(const VoidCallback& function);
	void SetOnPressed(const VoidCallback& function);
	void SetOnDown(const VoidCallback& function);
	void SetOnPressOut(const VoidCallback& function);

private:
	void OnMouseLeftButtonDown();

private:
	VoidCallback onClicked_;
	VoidCallback onPressed_;
	VoidCallback onDown_;
	VoidCallback onPressOut_;

	bool isPressed_;
};

