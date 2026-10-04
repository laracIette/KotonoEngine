#include "SceneContext.h"

#include <kotono_core/Scene.h>

WSceneContext::WSceneContext(UPath const& scenePath)
	: scenePath_{ scenePath }
	, scene_{ new UScene{ scenePath } }
{
}

WSceneContext::~WSceneContext()
{
	delete scene_;
}

void WSceneContext::Deserialize()
{
	Base::Deserialize();

	delete scene_;
	scene_ = new UScene{ scenePath_ };
}

void WSceneContext::Update(f32 deltaTime) const
{
	if (GetScene())
	{
		GetScene()->Update(deltaTime);
	}
}

#include "generated/SceneContext.generated.inl"
