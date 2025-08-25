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

	void Rebuild();

	// Get the first dirty widget encountered in the widget tree
	virtual WWidget* GetDirty();

	glm::vec2 GetPosition() const;
	glm::vec2 GetSize() const;

protected:
	bool isDirty_;

	void SetState(const StateFunction& function);

private:
	UBuildSettings buildSettings_;
	KtCached<WWidget*> cachedBuild_;
};

