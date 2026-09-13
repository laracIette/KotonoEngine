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
	//WInterfaceRoot() = delete; // register_ breaks if default constructor deleted
	WInterfaceRoot(UInterface* inInterface);

	void UpdateSceneContexts(f32 deltaTime) const;

	void PopulateSceneRenderGraph(USceneRenderGraph& sceneRenderGraph) const;

	UInterface* GetInterface() const override { assert(interface_); return interface_; }

protected:
	void AddSceneContext(UPtr<WSceneContext> const& sceneContext);
	void RemoveSceneContext(UPtr<WSceneContext> const& sceneContext);

private:
	UInterface* interface_;
	USet<UPtr<WSceneContext>> sceneContexts_;
};