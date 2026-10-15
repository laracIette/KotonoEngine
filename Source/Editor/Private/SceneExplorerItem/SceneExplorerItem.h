#pragma once
#include <SceneWidget/SceneWidget.h>

#include "SceneExplorerItem.generated.h"

class TSceneObject;

class WSceneExplorerItem : public WSceneWidget
{
	GENERATED()

public:
	WSceneExplorerItem(UScene* scene, UObjectPtr<TSceneObject> const& sceneObject);

protected:
	WidgetPtr Build() override;

private:
	UObjectPtr<TSceneObject> sceneObject_;
};