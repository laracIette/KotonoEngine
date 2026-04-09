#include "Text.h"
#include "widgets.h"
#include <kotono_graphics/Font.h>

WidgetPtr WText::Build()
{
	if (shouldWrap_)
	{
		return new WHorizontalWrapList({
			.itemSpacing = spacing_,
			.rowSpacing = 0.0f,
			.children = [this]() { return GetCharacters(); },
		});
	}
	else
	{
		return new WRow({
			.spacing = spacing_,
			.children = GetCharacters(),
		});
	}
}

UWidgetDisplaySettings WText::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	return Base::GetDisplaySettings(displaySettings);
}

const std::string& WText::GetText() const
{
	return text_;
}

const glm::vec2& WText::GetFontSize() const
{
	return fontSize_;
}

float WText::GetSpacing() const
{
	return spacing_;
}

bool WText::GetShouldWrap() const
{
	return shouldWrap_;
}

void WText::SetText(const std::string& text)
{
	text_ = text;
}

void WText::SetFontSize(const glm::vec2& fontSize)
{
	fontSize_ = fontSize;
}

void WText::SetSpacing(const float spacing)
{
	spacing_ = spacing;
}

void WText::SetShouldWrap(const bool shouldWrap)
{
	shouldWrap_ = shouldWrap;
}

WidgetVector WText::GetCharacters() const
{
	WidgetVector result{};

	const KtFont font("${ENGINE_DIRECTORY}/Graphics/assets/fonts/default");

	const auto characterPaths{ font.GetTextPaths(text_) };
	result.reserve(characterPaths.size());

	for (const auto& characterPath : characterPaths)
	{
		result.push_back(new WBox({
			.size = fontSize_,
			.child = new WImage({
				.path = characterPath,
			}),
		}));
	}

	return result;
}

#include "generated/Text.generated.inl"
