#include "SceneExplorerAddButton.h"

#include <kotono_interface/widgets.h>

WidgetPtr WSceneExplorerAddButton::Build()
{
	return (
		UCreate<WStack>{ "Add Button Stack" }()
		| (
			UCreate<WButton>{}()
			| Apply(&WButton::SetNormalColor, Colors::Green.WithValue(0.8f))
			| Apply(&WButton::SetDisabledColor, Colors::Green.WithValue(0.2f))
			| Apply(&WButton::SetOnClicked, []() { throw std::runtime_error{ "unimplemented!" }; })
		)
		| (
			UCreate<WCenter>{}()
			| (
				UCreate<WText>{}()
				| Apply(&WText::SetText, "Add")
				| Apply(&WText::SetFontSize, glm::vec2{ 22.0f, 26.0f })
			)
		)
	);
}

#include "generated/SceneExplorerAddButton.generated.inl"
