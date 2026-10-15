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
#include <Color.h>
#include <InputState.h>
#include <Key.h>
#include <Modifier.h>
#include <string>
#include <vector>

#include "Widget.generated.h"

inline constexpr b8 INPUT_HANDLED{ true };
inline constexpr b8 INPUT_UNHANDLED{ false };

class WWidget;
using WidgetPtr = UObjectPtr<WWidget>;
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
ABSTRACT class WWidget : public KObject
{
	GENERATED()

protected:
	using StateFunction = VoidCallback;
	using DisownCallback = std::function<void(WidgetPtr)>;

public:
	WWidget();
	~WWidget() override;

protected:
	/// Create the widget tree to display
	virtual WidgetPtr Build();
	
	/// Called right after Build()
	virtual void Init();
	
	virtual void OnDisplayed();
	virtual void OnRemoved();
	
	virtual void OnFocused();
	virtual void OnUnfocused();
	
	virtual void OnActive();
	virtual void OnInactive();

public:
	/// Start displaying the widget
	virtual void Display(UWidgetDisplaySettings displaySettings);
	/// Stop displaying the widget
	virtual void Remove();

	virtual void Disown(WidgetPtr const& widget);

	virtual auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2;
	virtual auto GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2;

	virtual auto GetExpand() const -> EExpand;
	virtual auto GetFlex() const -> EFlex;

	virtual auto GetInterface() const -> UInterface*;

	virtual void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph, UColor const& tint) const;
	virtual void PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const;
	
	virtual auto OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8;
	virtual auto OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) -> b8;
	virtual auto OnMouseScroll(glm::vec2 const& delta) -> b8;

	virtual auto OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) -> b8;

	virtual void Refresh();
	
	virtual void CacheBuild();
	
	void SetFocused();
	void SetUnfocused();
	
	void SetActive();
	void SetInactive();

	auto GetClassPath() const -> std::string;

	auto GetHasFlex(EFlex flex) const -> b8;
	auto GetHasExpand(EExpand expand) const -> b8;
	auto GetIsPointHovering(glm::vec2 const& position) const -> b8;

	GetterAndSetter(UColor, UColor, Tint, Tint);
	GetterAndSetter(b8, UBindable<b8>, isVisible_, IsVisible);
	
	Getter(glm::vec2, slotDisplaySettings_.position, Position);
	Getter(glm::vec2, slotDisplaySettings_.bounds, Size);
	Getter(f32, slotDisplaySettings_.bounds.x / slotDisplaySettings_.bounds.y, AspectRatio);
	Getter(UScissor, slotDisplaySettings_.scissor, Scissor);

protected:	
	void SetState(StateFunction const& function);

	virtual auto GetCanCache() const -> b8;
	
	auto GetShouldRefresh() const -> b8;
	auto GetIsContentVisible() const -> b8;
	
	void OnWidgetDisowned(WidgetPtr const& widget) const;

private:
	auto HasBuild() const -> b8;
	void MarkDirty();

	auto TranslationMatrix() const -> glm::mat4;
	static auto RotationMatrix() -> glm::mat4;
	auto ScaleMatrix() const -> glm::mat4;
	auto ModelMatrix() const -> glm::mat4;
	
protected:
	/// The tint of the widget, this propagates to the widget's content
	UColor Tint;

private:
	WidgetPtr build_;
	UWidgetDisplaySettings slotDisplaySettings_;
	b8 isDirty_;
	UBindable<b8> isVisible_;

	WritableProperty(WidgetPtr, parent_, Parent, Value);
	WritableProperty(DisownCallback, onDisowned_, OnDisowned);
	/// Whether visibility changes should propagate to the widget's content
	WritableProperty(b8, propagateVisibility_, PropagateVisibility);
	/// Whether the widget is currently displayed on screen
	ReadonlyProperty(b8, isDisplayed_, IsDisplayed);
	/// Whether the widget is currently focused by the controller
	ReadonlyProperty(b8, isFocused_, IsFocused);
	ReadonlyProperty(b8, isActive_, IsActive);
	ReadonlyProperty(glm::vec2, contentSize_, ContentSize);
	ReadonlyProperty(glm::mat4, modelMatrix_, ModelMatrix);
	StateProperty(glm::vec2, expandWeight_, ExpandWeight);
};
