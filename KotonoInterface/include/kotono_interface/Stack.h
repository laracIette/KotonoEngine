#pragma once
#include "ChildrenOwnerWidget.h"
#include <vector>
class WStack : public WChildrenOwnerWidget
{
	friend class VStackView;

public:
	struct StackSettings
	{
		std::vector<WWidget*> children{};
	};

	/// Display widgets on top of each other
	WStack(const StackSettings& stackSettings);

	VView* CreateView() override;

	void Destroy() override;

protected:
	StackSettings stackSettings_;
};

