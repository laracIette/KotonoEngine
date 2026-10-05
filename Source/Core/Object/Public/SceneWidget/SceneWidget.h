#pragma once
#include "SceneWidget.generated.h"
#include "Widget/Widget.h"

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