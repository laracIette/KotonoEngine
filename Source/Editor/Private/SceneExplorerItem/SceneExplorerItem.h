#pragma once
#include "generated/SceneExplorerItem.generated.h"
#include <SceneWidget/SceneWidget.h>

class TSceneObject;

class WSceneExplorerItem : public WSceneWidget
{
	GENERATED_WSCENEEXPLORERITEM()

public:
	WSceneExplorerItem(UScene* scene, UPtr<TSceneObject> const& sceneObject);

protected:
	WidgetPtr Build() override;

private:
	UPtr<TSceneObject> sceneObject_;
};