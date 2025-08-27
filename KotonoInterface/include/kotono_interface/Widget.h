#pragma once
#include <functional>
#include "utils.h"
#include <kotono_framework/Cached.h>
#include <kotono_framework/Event.h>
/// Base class of all widgets
class WWidget
{
	friend class VView;

	using StateFunction = std::function<void()>;

public:
	WWidget();
	virtual ~WWidget() = default;

	virtual WWidget* Build();
	virtual VView* CreateView();

	virtual void Cleanup();

	glm::vec2 GetPosition() const;
	glm::vec2 GetSize() const;

protected:
	void SetState(const StateFunction& function);

private:
	UBuildSettings buildSettings_;
	KtCached<WWidget*> cachedBuild_;

	void Rebuild();
};

