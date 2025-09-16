#include "Text.h"
#include "widgets.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Font.h>

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

std::vector<WWidget*> WText::GetCharacters() const
{
	std::vector<WWidget*> result{};

	const auto path{ Framework.GetPath().GetSolutionPath() / R"(assets\fonts\default)" };
	const KtFont font(path);

	const auto characterPaths{ font.GetTextPaths(textSettings_.text) };
	result.reserve(characterPaths.size());

	for (const auto& path : characterPaths)
	{
		result.push_back(new WBox({
			.size = textSettings_.fontSize,
			.child = new WImage({
				.path = path,
			}),
		}));
	}

	return result;
}
