#pragma once
#include "Widget/Widget.h"

#include "SceneWidget.generated.h"

class UScene;

ABSTRACT class WSceneWidget : public WWidget
{
	GENERATED()

public:
	WSceneWidget();

protected:
    void Init() override;

private:
	WritableProperty(UScene*, scene_, Scene);
};