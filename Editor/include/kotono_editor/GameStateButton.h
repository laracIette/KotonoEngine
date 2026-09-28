#pragma once
#include "generated/GameStateButton.generated.h"
#include <kotono_core/SceneWidget.h>
class WGameStateButton final : public WSceneWidget
{
	GENERATED_WGAMESTATEBUTTON()

protected:
	WidgetPtr Build() override;
};

