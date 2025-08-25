#pragma once
#include "ChildOwnerWidget.h"
class WCenter : public WChildOwnerWidget
{
	friend class VCenterView;

public:
	enum class Axis
	{
		All,
		Horizontal,
		Vertical,
	};

	struct CenterSettings
	{
		/// default  = Axis::All
		Axis axis{ Axis::All };
		WWidget* child{ nullptr };
	};

	/// Center the child widget on an Axis
	WCenter(const CenterSettings& centerSettings);

	VView* CreateView() override;

protected:
	CenterSettings centerSettings_;
};

