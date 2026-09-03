#include "SceneExplorerAddButton.h"

#include <kotono_interface/widgets.h>

WidgetPtr WSceneExplorerAddButton::Build()
{
	UPtr color{ UCreate<WColor>{}() };
	color->SetColor(Colors::Green.WithValue(0.8f));

	UPtr text{ UCreate<WText>{}() };
	text->SetFontSize({ 22.0f, 26.0f });
	text->SetText("Add");

	UPtr button{ UCreate<WButton>{}() };
	button->SetOnPressed([]() {
		throw std::runtime_error{ "unimplemented!" };
	});

	const auto widgetTree{ UChildrenOwnerTree{ UCreate<WStack>{ "Add Button Stack" }(), {
		new UWidgetTreeLeaf{ button },
		new UChildOwnerTree{ UCreate<WCenter>{}(),
			new UWidgetTreeLeaf{ text },
		},
		new UWidgetTreeLeaf{ color },
	} } };
	widgetTree.Link();

	return widgetTree.Widget();
}

#include "generated/SceneExplorerAddButton.generated.inl"
