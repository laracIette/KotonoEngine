#pragma once
#include "generated/InterfaceRoot.generated.h"
#include "Widget.h"
#include <cassert>
struct USceneRenderGraph;
class WSceneContext;
class WInterfaceRoot : public WWidget
{
	GENERATED_WINTERFACEROOT()

public:
	void UpdateSceneContexts(f32 deltaTime) const;

	void PopulateSceneRenderGraph(USceneRenderGraph& sceneRenderGraph) const;

	auto GetInterface() const -> UInterface* override { assert(interface_); return interface_; }
	void SetInterface(UInterface* inInterface) { interface_ = inInterface; }

protected:
	void AddSceneContext(UPtr<WSceneContext> const& sceneContext);
	void RemoveSceneContext(UPtr<WSceneContext> const& sceneContext);

private:
	UInterface* interface_;
	USet<UPtr<WSceneContext>> sceneContexts_;
};