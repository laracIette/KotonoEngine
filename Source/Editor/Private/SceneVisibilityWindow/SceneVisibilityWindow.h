#pragma once
#include "generated/SceneVisibilityWindow.generated.h"
#include <Widget/Widget.h>

#include <SceneVisibility.h>

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

