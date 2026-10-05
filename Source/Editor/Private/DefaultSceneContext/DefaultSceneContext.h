#pragma once
#include "DefaultSceneContext.generated.h"
#include <SceneContext/SceneContext.h>

class WDefaultSceneContext final : public WSceneContext
{
	GENERATED_WDEFAULTSCENECONTEXT()

protected:
	WidgetPtr Build() override;
};