#pragma once
#include "generated/SceneExplorerRemoveButton.generated.h"
#include <kotono_interface/Widget.h>
class WSceneExplorerRemoveButton final : public WWidget
{
	GENERATED_WSCENEEXPLORERREMOVEBUTTON()

public:
	WidgetPtr Build() override;
	void Cleanup() override;
};

