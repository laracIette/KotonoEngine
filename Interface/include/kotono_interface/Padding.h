#pragma once
#include "generated/Padding.generated.h"
#include "ChildOwner.h"

struct UPadding final
{
	float l, t, r, b;

	static constexpr UPadding Left(float left)     noexcept { return { left, 0.0f, 0.0f, 0.0f }; }
	static constexpr UPadding Top(float top)       noexcept { return { 0.0f, top, 0.0f, 0.0f }; }
	static constexpr UPadding Right(float right)   noexcept { return { 0.0f, 0.0f, right, 0.0f }; }
	static constexpr UPadding Bottom(float bottom) noexcept { return { 0.0f, 0.0f, 0.0f, bottom }; }

	static constexpr UPadding All(float v) noexcept { return { v, v, v, v }; }

	static consteval UPadding Zero() noexcept { return { 0.0f, 0.0f, 0.0f, 0.0f }; }

	constexpr UPadding WithLeft(float left)     const noexcept { return { left, t, r, b }; }
	constexpr UPadding WithTop(float top)       const noexcept { return { l, top, r, b }; }
	constexpr UPadding WithRight(float right)   const noexcept { return { l, t, right, b }; }
	constexpr UPadding WithBottom(float bottom) const noexcept { return { l, t, r, bottom }; }
};

/// Shrink the bounds of the child widget
class WPadding final : public WChildOwner
{
	GENERATED_WPADDING()

public:
	UWidgetDisplaySettings GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const override;
	glm::vec2 GetDesiredSize(const glm::vec2& bounds) const override;

private:
	StateProperty(UPadding, padding_, Padding);
};

