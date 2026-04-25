#pragma once
#include "generated/SceneExplorerItem.generated.h"
#include <kotono_interface/Widget.h>
#include <kotono_core/SceneObject.h>
class WSceneExplorerItem : public WWidget
{
	GENERATED_WSCENEEXPLORERITEM()

public:
	WSceneExplorerItem(const UPtr<TSceneObject>& sceneObject);
	
	WidgetPtr Build() override;

	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

private:
	UPtr<TSceneObject> sceneObject_;
};