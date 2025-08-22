#pragma once
#include <functional>
#include "utils.h"
#include <kotono_framework/Cached.h>
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

	virtual void Destroy();

	virtual WWidget* GetDirty();

	glm::vec2 GetPosition() const;
	glm::vec2 GetSize() const;

protected:
	bool isDirty_;

	void SetState(const StateFunction& function);

private:
	glm::vec2 position_;
	glm::vec2 size_;

	KtCached<WWidget*> cachedBuild_;

	WWidget* GetBuild();
};

