#pragma once
#include "generated/Widget.generated.h"
#include <kotono_object/Object.h>
#include "Axis.h"
#include "Expand.h"
#include "Flex.h"
#include <functional>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <kotono_common/types.h>
#include <kotono_graphics/Scissor.h>
#include <string>
#include <vector>

struct UWidgetDisplaySettings
{
	glm::vec2 position;
	glm::vec2 bounds;
	i32 layer;
	UScissor scissor;
};

class URenderContext;
class WWidget;
using WidgetPtr = UPtr<WWidget>;
using WidgetVector = std::vector<WidgetPtr>;
using WidgetPool = UPool<WidgetPtr>;

#define StateProperty(Type, Name, PropertyName) private:												\
	Type Name;																							\
public:																									\
	const Type& Get##PropertyName() const noexcept { return Name; }										\
	void Set##PropertyName(const Type& value) noexcept { SetState([this, value]() { Name = value; }); }	\
private:

/// Base class of all widgets
class WWidget : public KObject
{
	GENERATED_WWIDGET()

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
	virtual void Display(UWidgetDisplaySettings displaySettings);

	/// Stop displaying the widget
	virtual void Remove();

	virtual UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const;
	virtual glm::vec2 GetDesiredSize(const glm::vec2& bounds) const;

	virtual EExpand GetExpand() const;
	virtual EFlex GetFlex() const;

	virtual WidgetVector WidgetTree() const;
	std::string GetClassPath() const;

	bool IsMouseHovering() const;

	const glm::vec2& GetPosition() const;
	const glm::vec2& GetSize() const;
	u32 GetLayer() const;
	const UScissor& GetScissor() const;

	URenderContext* GetRenderContext() const;

protected:
	void SetState(const StateFunction& function);

	glm::mat4 TranslationMatrix() const;
	glm::mat4 RotationMatrix() const;
	glm::mat4 ScaleMatrix() const;
	glm::mat4 ModelMatrix() const;

	virtual void DisplayInternal(UWidgetDisplaySettings displaySettings);

	void Refresh();

private:
	void CacheBuild();
	bool HasBuild() const;
	bool IsRenderable(const UWidgetDisplaySettings& displaySettings) const;

#	ifndef NDEBUG
		void _OnMouseMove(const glm::vec2& delta);
#	endif
	WidgetPtr FindNonFlexAncestor(const EFlex flex) const;

private:
	WritableProperty(WidgetPtr, parent_, Parent);
	ReadonlyProperty(bool, isDisplayed_, IsDisplayed);
	// The display settings this widget's parent gave it
	ReadonlyProperty(UWidgetDisplaySettings, slotDisplaySettings_, SlotDisplaySettings);
	/// The actual display settings this widget uses
	ReadonlyProperty(UWidgetDisplaySettings, displaySettings_, DisplaySettings);
	bool wasMouseHovering_;
	WidgetPtr build_;
};

class UWidgetTree
{
public:
	virtual ~UWidgetTree() = default;

	virtual WidgetPtr Widget() const = 0;
	virtual void Link() const = 0;
};

class UWidgetTreeLeaf final : public UWidgetTree
{
public:
	UWidgetTreeLeaf(const WidgetPtr& widget);

	WidgetPtr Widget() const override;
	void Link() const override;

private:
	WidgetPtr widget_;
};
