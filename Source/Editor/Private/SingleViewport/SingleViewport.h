#pragma once
#include "generated/SingleViewport.generated.h"
#include <kotono_core/SceneWidget.h>
class WSingleViewport final : public WSceneWidget
{
	GENERATED_WSINGLEVIEWPORT()

protected:
	WidgetPtr Build() override;
};