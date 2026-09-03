#pragma once
#include "generated/SceneWidget.generated.h"
#include "Widget.h"
class UScene;
class WSceneContext;
class WSceneWidget : public WWidget
{
	GENERATED_WSCENEWIDGET()

public:
	WSceneWidget(UPtr<WSceneContext> const& sceneContext);

	UPtr<WSceneContext> const& GetSceneContext() const;
	UScene* GetScene() const;

private:
	SERIALIZE UPtr<WSceneContext> sceneContext_;
};