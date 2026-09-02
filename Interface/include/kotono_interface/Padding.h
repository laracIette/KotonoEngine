#pragma once
#include "generated/Padding.generated.h"
#include "ChildOwner.h"

struct UPadding final
{
	f32 l, t, r, b;

	static constexpr UPadding Left(f32 left)     noexcept { return { left, 0.0f, 0.0f, 0.0f }; }
	static constexpr UPadding Top(f32 top)       noexcept { return { 0.0f, top, 0.0f, 0.0f }; }
	static constexpr UPadding Right(f32 right)   noexcept { return { 0.0f, 0.0f, right, 0.0f }; }
	static constexpr UPadding Bottom(f32 bottom) noexcept { return { 0.0f, 0.0f, 0.0f, bottom }; }

	static constexpr UPadding All(f32 v) noexcept { return { v, v, v, v }; }

	static consteval UPadding Zero() noexcept { return { 0.0f, 0.0f, 0.0f, 0.0f }; }

	constexpr UPadding WithLeft(f32 left)     const noexcept { return { left, t, r, b }; }
	constexpr UPadding WithTop(f32 top)       const noexcept { return { l, top, r, b }; }
	constexpr UPadding WithRight(f32 right)   const noexcept { return { l, t, right, b }; }
	constexpr UPadding WithBottom(f32 bottom) const noexcept { return { l, t, r, bottom }; }
};

/// Shrink the bounds of the child widget
class WPadding final : public WChildOwner
{
	GENERATED_WPADDING()

public:
	WPadding(UPadding const& padding = UPadding::Zero());

public:
	glm::vec2 GetContentSize(glm::vec2 bounds) const override;
	glm::vec2 GetDesiredSize(glm::vec2 const& bounds) const override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	StateProperty(UPadding, padding_, Padding);
};

