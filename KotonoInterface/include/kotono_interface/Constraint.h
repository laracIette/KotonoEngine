#pragma once
#include "ChildOwnerWidget.h"
class WConstraint : public WChildOwnerWidget
{
public:
	enum class Axis
	{
		Horizontal,
		Vertical,
	};

	struct ConstraintSettings
	{
		// default = Axis::Horizontal
		Axis axis{ Axis::Horizontal };
		// default = 0.0f
		float size{ 0.0f };
		WWidget* child{ nullptr };
	};

	/// Constraints the bounds of the child widget given an axis
	WConstraint(const ConstraintSettings& constraintSettings);

	void Display(DisplaySettings displaySettings) override;

	DisplaySettings GetDisplaySettings(DisplaySettings displaySettings) override;

private:
	ConstraintSettings constraintSettings_;
};

