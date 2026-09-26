#pragma once
#include "generated/SceneExplorerRemoveButton.generated.h"
#include <kotono_core/SceneWidget.h>
class WSceneExplorerRemoveButton final : public WSceneWidget
{
	GENERATED_WSCENEEXPLORERREMOVEBUTTON()

protected:
	WidgetPtr Build() override;
};

