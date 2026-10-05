#pragma once
#include <SceneWidget/SceneWidget.h>

#include "SceneExplorerItem.generated.h"

class TSceneObject;

class WSceneExplorerItem : public WSceneWidget
{
	GENERATED()

public:
	WSceneExplorerItem(UScene* scene, UPtr<TSceneObject> const& sceneObject);

protected:
	WidgetPtr Build() override;

private:
	UPtr<TSceneObject> sceneObject_;
};