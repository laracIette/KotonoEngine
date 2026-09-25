#pragma once
#include "generated/Widget.generated.h"
#include "Object.h"

#include "Axis.h"
#include "Expand.h"
#include "Flex.h"
#include "WidgetDisplaySettings.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <kotono_common/Set.h>
#include <kotono_common/types.h>
#include <kotono_input/Button.h>
#include <kotono_input/InputState.h>
#include <kotono_input/Key.h>
#include <string>
#include <vector>

inline constexpr b8 INPUT_HANDLED{ true };
inline constexpr b8 INPUT_UNHANDLED{ false };

class WWidget;
using WidgetPtr = UPtr<WWidget>;
using WidgetSet = USet<WidgetPtr>;
using WidgetVector = std::vector<WidgetPtr>;

#define StateProperty(Type, Name, PropertyName, ...) private:											\
	Type Name;																							\
public:																									\
	Type GET_PROP_ACCESS(__VA_ARGS__) Get##PropertyName() GET_FUNC_ACCESS(__VA_ARGS__) { return Name; } \
	void Set##PropertyName(const Type& value) noexcept { SetState([this, value]() { Name = value; }); }	\
private:

struct UInterfaceRenderGraph;
class UInterface;

/// Base class of all widgets
class WWidget : public KObject
{
	GENERATED_WWIDGET()

private:
	friend class UInterface;

protected:
	using StateFunction = VoidCallback;

public:
	WWidget();
	~WWidget() override;

	void PostConstruct() override;

protected:
	/// Create the widget tree to display
	virtual WidgetPtr Build();

public:
	/// Start displaying the widget
	virtual void Display(UWidgetDisplaySettings const& displaySettings);

	/// Stop displaying the widget
	virtual void Remove();

	virtual void Disown(WidgetPtr const& widget);

	virtual auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2;
	virtual auto GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2;

	virtual auto GetExpand() const -> EExpand;
	virtual auto GetFlex() const -> EFlex;

	auto GetClassPath() const -> std::string;

	virtual auto GetInterface() const -> UInterface*;

	virtual void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const;
	virtual void PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const;

	virtual auto OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position) -> b8;
	virtual auto OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) -> b8;
	virtual auto OnMouseScroll(glm::vec2 const& delta) -> b8;

	virtual auto OnKeyboardKey(EKey key, EInputState inputState) -> b8;

	virtual void OnFocused();
	virtual void OnUnfocused();

	auto GetShouldRefresh() const -> b8;
	virtual void Refresh();

	auto GetPosition() const -> glm::vec2 { return slotDisplaySettings_.position; }
	auto GetSize() const -> glm::vec2 { return slotDisplaySettings_.bounds; }
	auto GetAspectRatio() const -> f32 { return slotDisplaySettings_.bounds.x / slotDisplaySettings_.bounds.y; }
	auto GetScissor() const -> UScissor { return slotDisplaySettings_.scissor; }

protected:
	void SetState(StateFunction const& function);

	virtual void DisplayInternal(UWidgetDisplaySettings displaySettings);

private:
	void CacheBuild();
	auto HasBuild() const -> b8;
	auto IsVisible(UWidgetDisplaySettings const& displaySettings) const -> b8;
	void MarkDirty();

	auto TranslationMatrix() const -> glm::mat4;
	auto RotationMatrix() const -> glm::mat4;
	auto ScaleMatrix() const -> glm::mat4;
	auto ModelMatrix() const -> glm::mat4;

private:
	WidgetPtr build_;
	UWidgetDisplaySettings slotDisplaySettings_;
	b8 isDirty_;
	WritableProperty(WidgetPtr, parent_, Parent);
	WritableProperty(b8, canCache_, CanCache, Value);
	WritableProperty(b8, isVisible_, IsVisible, Value);
	ReadonlyProperty(b8, isDisplayed_, IsDisplayed, Value);
	ReadonlyProperty(b8, isFocused_, IsFocused, Value);
	ReadonlyProperty(glm::vec2, contentSize_, ContentSize, Value);
	ReadonlyProperty(glm::mat4, modelMatrix_, ModelMatrix);
};
