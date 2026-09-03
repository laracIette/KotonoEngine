#include "SceneWidget.h"

#include "SceneContext.h"
#include <assert.h>

WSceneWidget::WSceneWidget(UPtr<WSceneContext> const& sceneContext)
	: sceneContext_{ sceneContext }
{
}

UPtr<WSceneContext> const& WSceneWidget::GetSceneContext() const
{
	return sceneContext_;
}

UScene* WSceneWidget::GetScene() const
{
	assert(sceneContext_);
	return sceneContext_->GetScene();
}

#include "generated/SceneWidget.generated.inl"
