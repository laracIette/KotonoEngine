#pragma once
#include "generated/SceneExplorerAddButton.generated.h"
#include <SceneWidget/SceneWidget.h>

class WSceneExplorerAddButton final : public WSceneWidget
{
	GENERATED_WSCENEEXPLORERADDBUTTON()

protected:
	WidgetPtr Build() override;
};