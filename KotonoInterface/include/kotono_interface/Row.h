#pragma once
#include "ChildrenOwnerWidget.h"
class WRow : public WChildrenOwnerWidget
{
	friend class VRowView;

public:
	struct RowSettings
	{
		/// default = 0.0f
		float spacing{ 0.0f };
		std::vector<WWidget*> children{};
	};

	/// Defines an horizontal container for widgets
	WRow(const RowSettings& rowSettings);	
	
	void Display(BuildSettings buildSettings) override;

protected:
	RowSettings rowSettings_;
};

