#pragma once
#include "generated/SceneExplorerItem.generated.h"
#include <kotono_object/Widget.h>
class WColor;
class TSceneObject;
class WSceneExplorerItem : public WWidget
{
	GENERATED_WSCENEEXPLORERITEM()

public:
	WSceneExplorerItem(UPtr<TSceneObject> const& sceneObject);
	
protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

private:
	void OnSelectedObjectChanged(UPtr<KObject> const& object);

private:
	UPtr<WColor> bg_;
	UPtr<TSceneObject> sceneObject_;
};