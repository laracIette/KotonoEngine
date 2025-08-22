#pragma once
#include "ChildOwnerWidget.h"
class WBounds : public WChildOwnerWidget
{
	friend class VBoundsView;

public:
	struct BoundsSettings
	{
		/// default = { 0.0f, 0.0f }
		glm::vec2 size{ 0.0f, 0.0f };
		WWidget* child{ nullptr };
	};

	/// Set bounds that the child should be contained within
	WBounds(const BoundsSettings& boundsSettings);

	VView* CreateView() override;

	void Destroy() override;

protected:
	BoundsSettings boundsSettings_;
};

