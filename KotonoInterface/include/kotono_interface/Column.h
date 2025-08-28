#pragma once
#include "ChildrenOwnerWidget.h"
#include <vector>
class WColumn : public WChildrenOwnerWidget
{
	friend class VColumnView;

public:
	struct ColumnSettings
	{
		/// default = 0.0f
		float spacing{ 0.0f };
		std::vector<WWidget*> children{};
	};

	/// Defines a vertical container for widgets
	WColumn(const ColumnSettings& columnSettings);

	void Display(BuildSettings buildSettings) override;

protected:
	ColumnSettings columnSettings_;
};

