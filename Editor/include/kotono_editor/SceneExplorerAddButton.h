#pragma once
#include "generated/SceneExplorerAddButton.generated.h"
#include <kotono_interface/Widget.h>
class WSceneExplorerAddButton final : public WWidget
{
	GENERATED_WSCENEEXPLORERADDBUTTON()

protected:
	WidgetPtr Build() override;
};