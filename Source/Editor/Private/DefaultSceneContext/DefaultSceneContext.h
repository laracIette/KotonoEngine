#pragma once
#include <SceneContext/SceneContext.h>

#include "DefaultSceneContext.generated.h"

class WDefaultSceneContext final : public WSceneContext
{
	GENERATED()

protected:
	WidgetPtr Build() override;
};