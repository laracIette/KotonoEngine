#pragma once
#include "generated/SceneExplorerAddButton.generated.h"
#include <kotono_object/SceneWidget.h>
class WSceneExplorerAddButton final : public WSceneWidget
{
	GENERATED_WSCENEEXPLORERADDBUTTON()

protected:
	WidgetPtr Build() override;
};