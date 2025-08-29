#pragma once
#include "ChildOwnerWidget.h"
class WBox : public WChildOwnerWidget
{
public:
	struct BoxSettings
	{
		/// default = { 0.0f, 0.0f }
		glm::vec2 size{ 0.0f, 0.0f };
		WWidget* child{ nullptr };
	};

	/// Set bounds for the child widget to be contained within
	WBox(const BoxSettings& boxSettings);

	void Display(DisplaySettings displaySettings) override;

	DisplaySettings GetDisplaySettings(DisplaySettings displaySettings) override;

protected:
	BoxSettings boxSettings_;
};

