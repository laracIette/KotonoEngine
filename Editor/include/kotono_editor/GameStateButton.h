#pragma once
#include "generated/GameStateButton.generated.h"
#include <kotono_interface\Widget.h>
class WGameStateButton : public WWidget
{
	GENERATED_WGAMESTATEBUTTON()

public:
	WidgetPtr Build() override;
	void Cleanup() override;

private:
	void OnKeyboardSpaceKeyPressed();
};

