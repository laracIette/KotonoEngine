#include "SceneWidget.h"

WSceneWidget::WSceneWidget(UScene* scene)
	: scene_{ scene }
{
}

WSceneWidget::WSceneWidget()
	: Self(nullptr)
{
}

#include "generated/SceneWidget.generated.inl"
