#pragma once
#include "generated/SceneExplorerRemoveButton.generated.h"
#include <SceneWidget/SceneWidget.h>

class WSceneExplorerRemoveButton final : public WSceneWidget
{
	GENERATED_WSCENEEXPLORERREMOVEBUTTON()

protected:
	WidgetPtr Build() override;
};

