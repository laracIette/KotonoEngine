#pragma once
#include "Ptr.h"
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/Set.h>
#include <kotono_timing/TimeContext.h>
struct UInterfaceRenderGraph;
struct USceneRenderGraph;
class WSceneContext;
class WWidget;
class UInterface final
{
public:
	UInterface();
	~UInterface();

	void PopulateInterfaceRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const;
	void PopulateSceneRenderGraph(USceneRenderGraph& sceneRenderGraph) const;

	void Update(f32 deltaTime);

	void BeginDraw(glm::uvec2 const& bounds);
	void EndDraw() const;

	glm::uvec2 const& GetBounds() const { return bounds_; }
	UTimeContext const& GetTimeContext() const { return timeContext_; }

	void SetWidget(UPtr<WSceneContext> const& widget) { widget_ = widget; }

private:
	void UpdateFocusedWidgets();

private:
	UPtr<WSceneContext> widget_;
	glm::uvec2 bounds_;

	UTimeContext timeContext_;

	USet<UPtr<WWidget>> focusedWidgets_;
};