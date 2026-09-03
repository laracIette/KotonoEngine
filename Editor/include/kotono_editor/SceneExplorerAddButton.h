#pragma once
#include "generated/SceneExplorerAddButton.generated.h"
#include <kotono_core/SceneWidget.h>
class WSceneExplorerAddButton final : public WSceneWidget
{
	GENERATED_WSCENEEXPLORERADDBUTTON()

protected:
	WidgetPtr Build() override;
};