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

void WSceneContext::Deserialize()
{
	Base::Deserialize();

	scene_ = new UScene{ scenePath_ };
}

void WSceneContext::Update(f32 deltaTime) const
{
	if (GetScene())
	{
		GetScene()->Update(deltaTime);
	}
}

void WSceneContext::PopulateSceneRenderGraph(USceneRenderGraph& sceneRenderGraph) const
{
	if (GetScene())
	{
		GetScene()->PopulateRenderGraph(sceneRenderGraph);
	}
}

#include "generated/SceneContext.generated.inl"
