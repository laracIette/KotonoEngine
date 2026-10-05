#pragma once
#include <SceneWidget/SceneWidget.h>

#include "SingleViewport.generated.h"

class WSingleViewport final : public WSceneWidget
{
	GENERATED()

protected:
	WidgetPtr Build() override;
};