#include "Text.h"
#include "widgets.h"
#include <kotono_graphics/Font.h>
#include <kotono_common/Path.h>
#include <kotono_common/log.h>

WText::WText(const TextSettings& textSettings) :
	textSettings_(textSettings)
{
}

WWidget* WText::Build()
{
	return new WRow({
		.spacing = textSettings_.spacing,
		.children = GetCharacters(),
	});
}

UWidgetDisplaySettings WText::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	const auto ds{ WWidget::GetDisplaySettings(displaySettings) };
	return ds;
}

WWidget::WidgetVector WText::GetCharacters() const
{
	WidgetVector result{};

	const KtFont font("${ENGINE_DIRECTORY}/Graphics/assets/fonts/default");

	const auto characterPaths{ font.GetTextPaths(textSettings_.text) };
	result.reserve(characterPaths.size());

	for (const auto& characterPath : characterPaths)
	{
		result.push_back(new WBox({
			.size = textSettings_.fontSize,
			.child = new WImage({
				.path = characterPath,
			}),
		}));
	}

	return result;
}
