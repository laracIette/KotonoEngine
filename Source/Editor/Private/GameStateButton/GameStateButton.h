#pragma once
#include "GameStateButton.generated.h"
#include <SceneWidget/SceneWidget.h>

class WGameStateButton final : public WSceneWidget
{
	GENERATED_WGAMESTATEBUTTON()

protected:
	WidgetPtr Build() override;
};

