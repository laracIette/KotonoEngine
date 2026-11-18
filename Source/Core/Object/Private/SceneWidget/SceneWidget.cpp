#include "SceneWidget/SceneWidget.h"

WSceneWidget::WSceneWidget()
	: scene_{ nullptr }
{
}

void WSceneWidget::Init()
{
    Base::Init();

    Check(Throw, scene_, "scene_ is null");
}

#include "SceneWidget.generated.inl"
