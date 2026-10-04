#pragma once
#include "generated/DefaultSceneContext.generated.h"
#include <kotono_core/SceneContext.h>
class WDefaultSceneContext final : public WSceneContext
{
	GENERATED_WDEFAULTSCENECONTEXT()

protected:
	WidgetPtr Build() override;
};