#pragma once
#include "ChildOwnerWidget.h"
class WPadding : public WChildOwnerWidget
{
public:
	struct Padding
	{
		float l, t, r, b;

		static constexpr Padding Left(float left)     noexcept { return { left, 0.0f, 0.0f, 0.0f }; }
		static constexpr Padding Top(float top)       noexcept { return { 0.0f, top, 0.0f, 0.0f }; }
		static constexpr Padding Right(float right)   noexcept { return { 0.0f, 0.0f, right, 0.0f }; }
		static constexpr Padding Bottom(float bottom) noexcept { return { 0.0f, 0.0f, 0.0f, bottom }; }

		static constexpr Padding All(float v) noexcept { return { v, v, v, v }; }

		static consteval Padding Zero() noexcept { return { 0.0f, 0.0f, 0.0f, 0.0f }; }

		constexpr Padding WithLeft(float left)     const noexcept { return { left, t, r, b }; }
		constexpr Padding WithTop(float top)       const noexcept { return { l, top, r, b }; }
		constexpr Padding WithRight(float right)   const noexcept { return { l, t, right, b }; }
		constexpr Padding WithBottom(float bottom) const noexcept { return { l, t, r, bottom }; }
	};

	struct PaddingSettings
	{
		/// default = Padding::Zero()
		Padding padding{ 0.0f, 0.0f, 0.0f, 0.0f };
		WidgetPtr child{ nullptr };
	};

	/// Shrink the bounds of the child widget
	WPadding(const PaddingSettings& paddingSettings);

	UWidgetDisplaySettings GetDisplaySettings(UWidgetDisplaySettings displaySettings) const override;

	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

protected:
	PaddingSettings paddingSettings_;

	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;
};

