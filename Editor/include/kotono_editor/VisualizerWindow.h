#pragma once
#include "generated/VisualizerWindow.generated.h"
#include <kotono_core/Widget.h>

#include <kotono_graphics/SceneVisibility.h>

class WVisualizerWindow : public WWidget
{
	GENERATED_WVISUALIZERWINDOW()

private:
	using SceneVisibilityChangedCallback = std::function<void(ESceneVisibility)>;

public:
	WVisualizerWindow();

protected:
	WidgetPtr Build() override;

private:
	ESceneVisibility sceneVisibility_;
	WritableProperty(SceneVisibilityChangedCallback, onSceneVisibilityChanged_, OnSceneVisibilityChanged);
};

