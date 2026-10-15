#pragma once
#include <SceneWidget/SceneWidget.h>

#include "PropertiesWindow.generated.h"

class TSceneObject;
class WList;
class WObjectProperties;

class WPropertiesWindow : public WSceneWidget
{
	GENERATED()

protected:
	WidgetPtr Build() override;

	void OnDisplayed() override;
	void OnRemoved() override;

private:
	void OnSelectedObjectChanged(UObjectPtr<TSceneObject> const& sceneObject);

private:
	UObjectPtr<WList> mainList_;
	UObjectPtr<WObjectProperties> objectProperties_;
};

