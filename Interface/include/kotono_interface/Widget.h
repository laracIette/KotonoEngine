#pragma once
#include "Flex.h"
#include "widget_fwd.h"
#include <functional>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <kotono_common/Cached.h>
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

/// Base class of all widgets
class WWidget
{
public:
	using StateFunction = std::function<void()>;
	using WidgetVector = std::vector<WidgetPtr>;

public:
	WWidget();
	virtual ~WWidget() = default;

	virtual void CacheBuild();

	virtual WidgetPtr Build();

	virtual void Cleanup();

	void Display(UWidgetDisplaySettings displaySettings);

	virtual UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const;

	virtual EFlex GetFlex() const;
	virtual glm::vec2 GetDesiredSize(glm::vec2 bounds) const;

	virtual WidgetVector GetWidgetTree();

	glm::vec2 Position() const;
	glm::vec2 Size() const;
	i32 Layer() const;

	bool IsMouseHovering() const;

	void SetParent(WidgetPtr parent);

protected:
	WidgetPtr parent_;
	UWidgetDisplaySettings displaySettings_;

	void SetState(const StateFunction& function);
	void SetDisplaySettings(const UWidgetDisplaySettings& displaySettings);

	glm::mat4 TranslationMatrix() const;
	glm::mat4 RotationMatrix() const;
	glm::mat4 ScaleMatrix() const;
	glm::mat4 ModelMatrix() const;

	virtual void DisplayInternal(UWidgetDisplaySettings displaySettings);

	void Refresh();

private:
	KtCached<WidgetPtr> cachedBuild_;

	void Rebuild();
};

