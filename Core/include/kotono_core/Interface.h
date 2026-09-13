#pragma once
#include "Ptr.h"
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/Set.h>
struct UInterfaceRenderGraph;
struct USceneRenderGraph;
class WInterfaceRoot;
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

	glm::uvec2 GetBounds() const { return bounds_; }

	f32 GetDeltaTime() const { return deltaTime_; }
	f32 GetNow() const { return now_; }

	void SetWidget(UPtr<WInterfaceRoot> const& widget) { widget_ = widget; }

private:
	void UpdateFocusedWidgets();

private:
	UPtr<WInterfaceRoot> widget_;
	glm::uvec2 bounds_;

	f32 deltaTime_;
	f32 now_;

	/// Widgets currently hovered by the mouse cursor
	USet<UPtr<WWidget>> focusedWidgets_;
};