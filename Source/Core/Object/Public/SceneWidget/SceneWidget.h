#pragma once
#include "Widget/Widget.h"

#include "SceneWidget.generated.h"

class UScene;

ABSTRACT class WSceneWidget : public WWidget
{
	GENERATED()

public:
	//WSceneWidget() = delete; // register_ breaks if default constructor deleted
	WSceneWidget();
	WSceneWidget(UScene* scene);

private:
	ReadonlyProperty(UScene*, scene_, Scene);
};