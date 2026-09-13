#include "InterfaceRoot.h"

#include "SceneContext.h"

WInterfaceRoot::WInterfaceRoot(UInterface* inInterface)
	: interface_{ inInterface }
{
}

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

void WInterfaceRoot::PopulateSceneRenderGraph(USceneRenderGraph& sceneRenderGraph) const
{
	for (auto const& sceneContext : sceneContexts_)
	{
		if (sceneContext)
		{
			sceneContext->PopulateSceneRenderGraph(sceneRenderGraph);
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
