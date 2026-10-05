#pragma once
#include "SceneExplorerAddButton.generated.h"
#include <SceneWidget/SceneWidget.h>

class WSceneExplorerAddButton final : public WSceneWidget
{
	GENERATED_WSCENEEXPLORERADDBUTTON()

protected:
	WidgetPtr Build() override;
};