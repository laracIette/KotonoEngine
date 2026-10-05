#include "GameSpeedButton.h"

#include <Scene/Scene.h>
#include <core_widgets.h>

WidgetPtr WGameSpeedButton::Build()
{
	return (
		UCreate<WRow>{}()
		| (
			UCreate<WStack>{}()
			| (
				UCreate<WButton>{ "Speed Up Button" }()
				| Apply(&WButton::SetNormalColor, Colors::Transparent)
				| Apply(&WButton::SetPressedColor, Colors::Transparent)
				| Apply(&WButton::SetFocusedColor, Colors::Transparent)
				| Apply(&WButton::SetOnDrag, [this](glm::vec2 const& delta) { 
					auto const scale{ GetScene()->GetTimeScale() };
					GetScene()->SetTimeScale(scale + delta.x * 0.01f);
				})
			)
			| (
				UCreate<WText>{ "Game Speed Text" }()
				| Apply(&WText::SetText, [this]() { 
					return std::format("x{0:.2f}", GetScene()->GetTimeScale());
				})
			)
		)
		| (
			UCreate<WConstraint>{}()
			| Apply(&WConstraint::SetAxis, EAxis::Horizontal)
			| Apply(&WConstraint::SetSize, 32.0f)
			| (
				UCreate<WButton>{ "Reset Button" }()
				| Apply(&WButton::SetIsEnabled, [this]() { return GetScene()->GetTimeScale() != 1.0f; })
				| Apply(&WButton::SetOnClicked, [this]() { GetScene()->SetTimeScale(1.0f); })
			)
		)
	);
}

#include "GameSpeedButton.generated.inl"
