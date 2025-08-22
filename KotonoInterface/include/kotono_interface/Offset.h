#pragma once
#include "ChildOwnerWidget.h"
class WOffset : public WChildOwnerWidget
{
	friend class VOffsetView;

public:
	struct OffsetSettings
	{
		/// default = { 0.0f, 0.0f }
		glm::vec2 size{ 0.0f, 0.0f };
		WWidget* child{ nullptr };
	};

	/// Offset the position of the child widget
	WOffset(const OffsetSettings& offsetSettings);

	VView* CreateView() override;

	void Destroy() override;

protected:
	OffsetSettings offsetSettings_;
};

