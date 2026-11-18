#pragma once

#include "SceneWidget/SceneWidget.h"

#include "ObjectPropertiesWindow.generated.h"

class KSceneObject;
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
	void OnSelectedObjectsUpdated(USpan<UObjectPtr<KSceneObject> const> sceneObjects);

private:
	UObjectPtr<WSocket> objectPropertiesSocket_;
	UObjectPtr<WObjectProperties> objectProperties_;
};

