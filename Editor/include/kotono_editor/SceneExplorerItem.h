#pragma once
#include "generated/SceneExplorerItem.generated.h"
#include <kotono_interface/Widget.h>
#include <kotono_core/SceneObject.h>
class WColor;
class WSceneExplorerItem : public WWidget
{
	GENERATED_WSCENEEXPLORERITEM()

public:
	WSceneExplorerItem(const UPtr<TSceneObject>& sceneObject);
	
protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

private:
	void OnSelectedObjectChanged(const UPtr<KObject> object);

private:
	UPtr<WColor> bg_;
	UPtr<TSceneObject> sceneObject_;
};