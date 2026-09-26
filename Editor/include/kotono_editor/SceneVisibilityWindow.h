#pragma once
#include "generated/SceneVisibilityWindow.generated.h"
#include <kotono_core/Widget.h>

#include <kotono_graphics/SceneVisibility.h>

class WSceneVisibilityWindow : public WWidget
{
	GENERATED_WSCENEVISIBILITYWINDOW()

private:
	using SceneVisibilityChangedCallback = std::function<void(ESceneVisibility)>;

public:
	WSceneVisibilityWindow();
	WSceneVisibilityWindow(ESceneVisibility sceneVisibility);

protected:
	WidgetPtr Build() override;

private:
	ESceneVisibility sceneVisibility_;
	WritableProperty(SceneVisibilityChangedCallback, onSceneVisibilityChanged_, OnSceneVisibilityChanged);
};

