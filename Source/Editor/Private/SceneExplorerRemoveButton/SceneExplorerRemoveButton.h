#pragma once
#include <SceneWidget/SceneWidget.h>

#include "SceneExplorerRemoveButton.generated.h"

class WSceneExplorerRemoveButton final : public WSceneWidget
{
	GENERATED()

protected:
	WidgetPtr Build() override;
};

