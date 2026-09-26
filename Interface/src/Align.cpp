#include "Align.h"

WAlign::WAlign()
	: alignment_{ UAlignment::Center() }
{
}

auto WAlign::GetContentSize(glm::vec2 const& bounds) const -> glm::vec2
{
	return bounds;
}

auto WAlign::GetFlex() const -> EFlex
{
	return EFlex::All;
}

auto WAlign::GetExpand() const -> EExpand
{
	return EExpand::All;
}

void WAlign::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (GetChild())
	{
		const auto childSize{ GetChild()->GetContentSize(displaySettings.bounds) };

		auto const min{ displaySettings.position };
		auto const max{ displaySettings.position + displaySettings.bounds - childSize };

		displaySettings.position = glm::mix(min, max, glm::vec2{ alignment_.x, alignment_.y });
	}

	Base::DisplayInternal(displaySettings);
}

#include "generated/Align.generated.inl"
