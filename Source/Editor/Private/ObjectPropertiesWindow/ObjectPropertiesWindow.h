#pragma once
#include <SceneWidget/SceneWidget.h>

#include "ObjectPropertiesWindow.generated.h"

class TSceneObject;
class WObjectProperties;
class WSocket;

class WObjectPropertiesWindow : public WSceneWidget
{
	GENERATED()

protected:
	WidgetPtr Build() override;

	void OnDisplayed() override;
	void OnRemoved() override;

private:
	void OnSelectedObjectsUpdated(std::span<UObjectPtr<TSceneObject> const> sceneObjects);

private:
	UObjectPtr<WSocket> objectPropertiesSocket_;
	UObjectPtr<WObjectProperties> objectProperties_;
};

