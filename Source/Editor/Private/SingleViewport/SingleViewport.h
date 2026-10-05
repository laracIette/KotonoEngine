#pragma once
#include "SingleViewport.generated.h"
#include <SceneWidget/SceneWidget.h>

class WSingleViewport final : public WSceneWidget
{
	GENERATED_WSINGLEVIEWPORT()

protected:
	WidgetPtr Build() override;
};