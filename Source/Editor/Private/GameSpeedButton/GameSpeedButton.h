#pragma once
#include "generated/GameSpeedButton.generated.h"
#include <SceneWidget/SceneWidget.h>

class WGameSpeedButton final : public WSceneWidget
{
	GENERATED_WGAMESPEEDBUTTON()

protected:
	WidgetPtr Build() override;
};