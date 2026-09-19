#pragma once
#include "generated/SceneExplorerItem.generated.h"
#include <kotono_core/SceneWidget.h>
class TSceneObject;
class WSceneExplorerItem : public WSceneWidget
{
	GENERATED_WSCENEEXPLORERITEM()

public:
	WSceneExplorerItem(UScene* scene, UPtr<TSceneObject> const& sceneObject);

protected:
	WidgetPtr Build() override;

public:
	void Display(UWidgetDisplaySettings const& displaySettings) override;
	void Remove() override;

private:
	void OnSelectedObjectChanged(UPtr<TSceneObject> const& sceneObject);

private:
	UPtr<TSceneObject> sceneObject_;
};