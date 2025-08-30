#pragma once
#include "ChildOwnerWidget.h"
class WPadding : public WChildOwnerWidget
{
public:
	struct Padding
	{
		float l, t, r, b;

		constexpr Padding WithLeft(float left)     const noexcept { return { left, t, r, b }; }
		constexpr Padding WithTop(float top)       const noexcept { return { l, top, r, b }; }
		constexpr Padding WithRight(float right)   const noexcept { return { l, t, right, b }; }
		constexpr Padding WithBottom(float bottom) const noexcept { return { l, t, r, bottom }; }

		static constexpr Padding All(float v) noexcept { return { v, v, v, v }; }

		static consteval Padding Zero() noexcept { return { 0.0f, 0.0f, 0.0f, 0.0f }; }
	};

	struct PaddingSettings
	{
		/// default = Padding::Zero()
		Padding padding{ 0.0f, 0.0f, 0.0f, 0.0f };
		WWidget* child{ nullptr };
	};

	/// Shrink the bounds of the child widget
	WPadding(const PaddingSettings& paddingSettings);

	void Display(DisplaySettings displaySettings) override;

	DisplaySettings GetDisplaySettings(DisplaySettings displaySettings) const override;

protected:
	PaddingSettings paddingSettings_;
};

