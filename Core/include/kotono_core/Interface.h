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
	
	//void CreateWindow(glm::uvec2 const& position, glm::uvec2 const& size, UPtr<WWidget> const& widget);

	auto GetBounds() const -> glm::uvec2 { return bounds_; }

	auto GetDeltaTime() const -> f32 { return deltaTime_; }
	auto GetNow() const -> f32 { return now_; }

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