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

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	EFlex GetFlex() const override;
	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

protected:
	BoxSettings boxSettings_;

	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;
};

