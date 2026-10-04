#pragma once
#include "generated/SceneWidget.generated.h"
#include "Widget.h"
class UScene;
class WSceneWidget : public WWidget
{
	GENERATED_WSCENEWIDGET()

public:
	//WSceneWidget() = delete; // register_ breaks if default constructor deleted
	WSceneWidget();
	WSceneWidget(UScene* scene);

private:
	ReadonlyProperty(UScene*, scene_, Scene, Value);
};