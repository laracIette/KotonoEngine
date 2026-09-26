#pragma once
#include "generated/Align.generated.h"
#include "ChildOwner.h"

struct UAlignment final
{
	f32 x, y;

	static consteval UAlignment Center() noexcept { return { 0.5f, 0.5f }; }

	static consteval UAlignment Left()		noexcept { return { 0.0f, 0.5f }; }
	static consteval UAlignment Top()		noexcept { return { 0.5f, 0.0f }; }
	static consteval UAlignment Right()		noexcept { return { 1.0f, 0.5f }; }
	static consteval UAlignment Bottom()	noexcept { return { 0.5f, 1.0f }; }

	static consteval UAlignment TopLeft()		noexcept{ return { 0.0f, 0.0f }; }
	static consteval UAlignment TopRight()		noexcept { return { 1.0f, 0.0f }; }
	static consteval UAlignment BottomLeft()	noexcept { return { 0.0f, 1.0f }; }
	static consteval UAlignment BottomRight()	noexcept { return { 1.0f, 1.0f }; }

	constexpr UAlignment PadLeft(f32 l)		const noexcept { return { x + l, y }; }
	constexpr UAlignment PadTop(f32 t)		const noexcept { return { x, y + t }; }
	constexpr UAlignment PadRight(f32 r)	const noexcept { return { x - r, y }; }
	constexpr UAlignment PadBottom(f32 b)	const noexcept { return { x, y - b }; }
};

class WAlign final : public WChildOwner
{
	GENERATED_WALIGN()

public:
	WAlign();

	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;

	auto GetFlex() const -> EFlex override;
	auto GetExpand() const -> EExpand override;

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	WritableProperty(UAlignment, alignment_, Alignment, Value);

};