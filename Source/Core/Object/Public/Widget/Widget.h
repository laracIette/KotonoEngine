#pragma once
#include "Object/Object.h"

#include "Axis.h"
#include "Expand.h"
#include "Flex.h"
#include "WidgetDisplaySettings.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <Bindable.h>
#include <Containers/Set.h>
#include <types.h>
#include <Button.h>
#include <InputState.h>
#include <Key.h>
#include <Modifier.h>
#include <string>
#include <vector>

#include "Widget.generated.h"

inline constexpr b8 INPUT_HANDLED{ true };
inline constexpr b8 INPUT_UNHANDLED{ false };

class WWidget;
using WidgetPtr = UPtr<WWidget>;
using WidgetSet = USet<WidgetPtr>;
using WidgetVector = std::vector<WidgetPtr>;

#define StateSetter(Type, Variable, SetterName) \
	void Set##SetterName(setter_param_t<Type> value) { SetState([this, value]() { Variable = value; }); }

#define StateProperty(Type, Name, PropertyName, ...) private:	\
	Type Name;													\
public:															\
	Getter(Type, Name, PropertyName, __VA_ARGS__)				\
	StateSetter(Type, Name, PropertyName)						\
private:

struct UInterfaceRenderGraph;
class UInterface;

/// Base class of all widgets
class WWidget : public KObject
{
	GENERATED()

private:
	friend class UInterface;

protected:
	using StateFunction = VoidCallback;

public:
	WWidget();
	~WWidget() override;

protected:
	/// Create the widget tree to display
	virtual WidgetPtr Build();
	
	/// Called right after Build()
	virtual void Init();

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

	virtual auto OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8;
	virtual auto OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) -> b8;
	virtual auto OnMouseScroll(glm::vec2 const& delta) -> b8;

	virtual auto OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) -> b8;

	virtual void OnFocused();
	virtual void OnUnfocused();

	virtual void Refresh();
	
	virtual void CacheBuild();

	auto GetIsPointHovering(glm::vec2 const& position) const -> b8;

	GetterAndSetter(b8, UBindable<b8>, isVisible_, IsVisible);
	
	Getter(glm::vec2, slotDisplaySettings_.position, Position);
	Getter(glm::vec2, slotDisplaySettings_.bounds, Size);
	Getter(f32, slotDisplaySettings_.bounds.x / slotDisplaySettings_.bounds.y, AspectRatio);
	Getter(UScissor, slotDisplaySettings_.scissor, Scissor);

protected:
	void SetState(StateFunction const& function);

	virtual void DisplayInternal(UWidgetDisplaySettings displaySettings);

	virtual auto GetCanCache() const -> b8;
	
	auto GetShouldRefresh() const -> b8;
	auto GetCanPopulateRenderGraph() const -> b8;

private:
	auto HasBuild() const -> b8;
	void MarkDirty();

	auto TranslationMatrix() const -> glm::mat4;
	static auto RotationMatrix() -> glm::mat4;
	auto ScaleMatrix() const -> glm::mat4;
	auto ModelMatrix() const -> glm::mat4;

private:
	WidgetPtr build_;
	UWidgetDisplaySettings slotDisplaySettings_;
	b8 isDirty_;
	UBindable<b8> isVisible_;

	WritableProperty(WidgetPtr, parent_, Parent, Value);
	/// Whether visibility changes should propagate to the underlying build
	WritableProperty(b8, propagateVisibility_, PropagateVisibility);
	/// Whether the widget is currently displayed on screen
	ReadonlyProperty(b8, isDisplayed_, IsDisplayed);
	/// Whether the widget is currently focused by the controller
	ReadonlyProperty(b8, isFocused_, IsFocused);
	ReadonlyProperty(glm::vec2, contentSize_, ContentSize);
	ReadonlyProperty(glm::mat4, modelMatrix_, ModelMatrix);
	StateProperty(glm::vec2, expandWeight_, ExpandWeight);
};
