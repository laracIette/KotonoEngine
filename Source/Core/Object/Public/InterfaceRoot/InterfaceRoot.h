#pragma once
#include "Widget/Widget.h"

#include "InterfaceRoot.generated.h"

struct USceneRenderGraph;
class WSceneContext;

class WInterfaceRoot : public WWidget
{
	GENERATED()

public:
	void UpdateSceneContexts(f32 deltaTime) const;

	auto GetInterface() const -> UInterface* override { return interface_; }
	void SetInterface(UInterface* inInterface) { interface_ = inInterface; }

protected:
	void AddSceneContext(UObjectPtr<WSceneContext> const& sceneContext);
	void RemoveSceneContext(UObjectPtr<WSceneContext> const& sceneContext);

private:
	UInterface* interface_;
	USet<UObjectPtr<WSceneContext>> sceneContexts_;
};