#pragma once
#include <SceneWidget/SceneWidget.h>

#include "SceneExplorerAddButton.generated.h"

class WSceneExplorerAddButton final : public WSceneWidget
{
	GENERATED()

protected:
	WidgetPtr Build() override;
};