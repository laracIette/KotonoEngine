#pragma once
#include "Ptr.h"
#include <Containers/Set.h>
#include <glm/ext/vector_uint2.hpp>
#include <glm/fwd.hpp>
#include <properties.h>
#include <span>
#include <string>
#include <vector>

enum class EButton : u8;
enum class EKey : u8;
enum class EInputState : u8;
enum class EModifier : u8;
struct UInterfaceRenderGraph;
struct USceneRenderGraph;
class WInterfaceRoot;
class WWidget;

class UInterface final
{
public:
	struct PendingWindow
	{
		glm::uvec2 windowExtent;
		UPtr<WWidget> widget;
		std::string name;
	};

public:
	UInterface();
	
	void Cleanup() const;

	void PopulateInterfaceRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const;

	void Update(f32 deltaTime, glm::vec2 const& cursorPosition);

	void BeginDraw(glm::uvec2 const& bounds);
	void EndDraw() const;

	void OnMouseButton(EButton button, EInputState inputState, EModifier modifier);
	void OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) const;
	void OnMouseScroll(glm::vec2 const& delta) const;

	void OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) const;
	
	void OpenWidgetInWindow(glm::uvec2 const& windowExtent, UPtr<WWidget> const& widget, std::string_view name);
	void ClearPendingWindows();

	Getter(std::span<PendingWindow const>, pendingWindows_, PendingWindows);

private:
	void UpdateFocusedWidgets(glm::vec2 const& cursorPosition);
	void UpdateActiveWidgets();

private:
	WritableProperty(UPtr<WInterfaceRoot>, widget_, Widget, Value);
	ReadonlyProperty(glm::uvec2, bounds_, Bounds);

	ReadonlyProperty(f32, deltaTime_, DeltaTime);
	ReadonlyProperty(f32, now_, Now);

	/// Widgets currently hovered by the mouse cursor
	USet<UPtr<WWidget>> focusedWidgets_;
	USet<UPtr<WWidget>> activeWidgets_;

	std::vector<PendingWindow> pendingWindows_;
};