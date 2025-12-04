#include "Text.h"
#include "widgets.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Font.h>
#include <kotono_common/Path.h>
#include "log.h"

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

WWidget::DisplaySettings WText::GetDisplaySettings(DisplaySettings displaySettings) const
{
	const auto ds = WWidget::GetDisplaySettings(displaySettings);
	return ds;
}

WWidget::WidgetVector WText::GetCharacters() const
{
	WidgetVector result{};

	const auto fontPath{ Framework.Path().Framework() / R"(assets\fonts\default)" };
	const KtFont font(fontPath);

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
