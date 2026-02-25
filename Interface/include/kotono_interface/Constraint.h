#pragma once
#include "ChildOwnerWidget.h"
#include "Axis.h"
class WConstraint : public WChildOwnerWidget
{
public:
	struct ConstraintSettings
	{
		/// default = EAxis::Horizontal
		EAxis axis{ EAxis::Horizontal };
		/// default = 0.0f
		float size{ 0.0f };
		WidgetPtr child{ nullptr };
	};

	/// Constraints the bounds of the child widget given an axis
	WConstraint(const ConstraintSettings& constraintSettings);

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	EFlex GetFlex() const override;
	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

protected:
	ConstraintSettings constraintSettings_;

	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;
};

