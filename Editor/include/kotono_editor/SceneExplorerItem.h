#pragma once
#include <kotono_interface/Widget.h>
#include <kotono_core/SceneObject.h>
class WSceneExplorerItem : public WWidget
{
public:
	WSceneExplorerItem(const UPtr<TSceneObject>& sceneObject);
	
	WidgetPtr Build() override;
	void Cleanup() override;

private:
	UPtr<TSceneObject> sceneObject_;
};