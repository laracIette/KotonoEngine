#pragma once
#include <Widget/Widget.h>

#include <SceneVisibility.h>

#include "SceneVisibilityWindow.generated.h"

class WSceneVisibilityWindow : public WWidget
{
	GENERATED()

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

