#pragma once
#include <SceneWidget/SceneWidget.h>

#include "GameStateButton.generated.h"

class WGameStateButton final : public WSceneWidget
{
	GENERATED()

protected:
	WidgetPtr Build() override;
};

