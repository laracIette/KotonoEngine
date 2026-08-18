#include "SceneContext.h"
#include <kotono_object/Scene.h>

WSceneContext::WSceneContext(UPath const& scenePath)
	: scenePath_{ scenePath }
	, scene_{ new UScene{ scenePath } }
{
}

WSceneContext::~WSceneContext()
{
	delete scene_;
}

UScene* WSceneContext::GetScene() const
{
	return scene_;
}

void WSceneContext::Deserialize()
{
	Base::Deserialize();

	scene_ = new UScene{ scenePath_ };
}

#include "generated/SceneContext.generated.inl"
