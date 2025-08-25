#pragma once
#include "ChildOwnerWidget.h"
class WPadding : public WChildOwnerWidget
{
	friend class VPaddingView;

public:
	struct Padding
	{
		float l, t, r, b;

		Padding WithLeft(const float left)     const noexcept { return { left, t, r, b }; }
		Padding WithTop(const float top)       const noexcept { return { l, top, r, b }; }
		Padding WithRight(const float right)   const noexcept { return { l, t, right, b }; }
		Padding WithBottom(const float bottom) const noexcept { return { l, t, r, bottom }; }

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

	VView* CreateView() override;

protected:
	PaddingSettings paddingSettings_;
};

