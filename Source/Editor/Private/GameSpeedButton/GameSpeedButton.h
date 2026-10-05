#pragma once
#include <SceneWidget/SceneWidget.h>

#include "GameSpeedButton.generated.h"

class WGameSpeedButton final : public WSceneWidget
{
	GENERATED()

protected:
	WidgetPtr Build() override;
};