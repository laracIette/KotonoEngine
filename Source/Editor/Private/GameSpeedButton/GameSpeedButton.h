#pragma once
#include "GameSpeedButton.generated.h"
#include <SceneWidget/SceneWidget.h>

class WGameSpeedButton final : public WSceneWidget
{
	GENERATED_WGAMESPEEDBUTTON()

protected:
	WidgetPtr Build() override;
};