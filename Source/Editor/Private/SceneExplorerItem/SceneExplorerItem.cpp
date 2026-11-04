#include "SceneExplorerItem.h"

#include <Scene/Scene.h>
#include <SceneObject/SceneObject.h>
#include <core_widgets.h>

WSceneExplorerItem::WSceneExplorerItem(UScene* scene, UObjectPtr<TSceneObject> const& sceneObject)
	: Base(scene)
	, sceneObject_{ sceneObject }
{
}

WidgetPtr WSceneExplorerItem::Build()
{
	return nullptr;
}

#include "SceneExplorerItem.generated.inl"
