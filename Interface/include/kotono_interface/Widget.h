#pragma once
#include "Flex.h"
#include <functional>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <kotono_common/Cached.h>
#include <kotono_common/types.h>
#include <kotono_graphics/Scissor.h>
#include <string>
/// Base class of all widgets
class WWidget
{
public:
	using StateFunction = std::function<void()>;
	using WidgetVector = std::vector<WWidget*>;
	using WidgetBuilder = std::function<WWidget*()>;
	using WidgetVectorBuilder = std::function<WidgetVector()>;

public:
	struct DisplaySettings
	{
		glm::vec2 position;
		glm::vec2 bounds;
		i32 layer;
		KtScissor scissor;
	};

	WWidget();
	virtual ~WWidget() = default;

	virtual void CacheBuild();

	virtual WWidget* Build();

	virtual void Cleanup();

	void Display(DisplaySettings displaySettings);

	virtual DisplaySettings GetDisplaySettings(DisplaySettings displaySettings) const;

	virtual EFlex GetFlex() const;

	virtual WidgetVector GetWidgetTree();

	glm::vec2 GetPosition() const;
	glm::vec2 GetSize() const;

	void SetParent(WWidget* parent);

protected:
	WWidget* parent_;
	DisplaySettings displaySettings_;

	void SetState(const StateFunction& function);
	void SetDisplaySettings(const DisplaySettings& displaySettings);

	glm::mat4 TranslationMatrix() const;
	glm::mat4 RotationMatrix() const;
	glm::mat4 ScaleMatrix() const;
	glm::mat4 ModelMatrix() const;

	virtual void DisplayInternal(DisplaySettings displaySettings);

	void Refresh();

private:
	KtCached<WWidget*> cachedBuild_;

	void Rebuild();
};

