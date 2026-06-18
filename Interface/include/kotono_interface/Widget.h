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
	KtScissor scissor;
};

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

	virtual WidgetVector GetWidgetTree();

	bool IsMouseHovering() const;

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
	bool IsNotBuild() const;
	bool IsRenderable(const UWidgetDisplaySettings& displaySettings) const;

	void OnMouseMove(const glm::vec2 delta);
	UPtr<WWidget> FindNonFlexAncestor(const EAxis axis) const;

private:
	WritableProperty(WidgetPtr, parent_, Parent);
	ReadonlyProperty(UWidgetDisplaySettings, displaySettings_, DisplaySettings);
	ReadonlyProperty(bool, isDisplayed_, IsDisplayed);
	ReadonlyProperty(glm::vec2, position_, Position);
	ReadonlyProperty(glm::vec2, size_, Size);
	ReadonlyProperty(i32, layer_, Layer);
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
