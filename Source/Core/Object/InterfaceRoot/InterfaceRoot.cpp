#include "InterfaceRoot.h"

#include "SceneContext.h"

void WInterfaceRoot::UpdateSceneContexts(f32 deltaTime) const
{
	for (auto const& sceneContext : sceneContexts_)
	{
		if (sceneContext)
		{
			sceneContext->Update(deltaTime);
		}
	}
}

void WInterfaceRoot::AddSceneContext(UPtr<WSceneContext> const& sceneContext)
{
	sceneContexts_.Add(sceneContext);
}

void WInterfaceRoot::RemoveSceneContext(UPtr<WSceneContext> const& sceneContext)
{
	sceneContexts_.Remove(sceneContext);
}

#include "generated/InterfaceRoot.generated.inl"
