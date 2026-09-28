#include "GameSpeedButton.h"

#include <kotono_core/Scene.h>
#include <kotono_interface/widgets.h>

WidgetPtr WGameSpeedButton::Build()
{
	return (
		UCreate<WRow>{}()
		| (
			UCreate<WButton>{ "Speed Up Button" }()
			| Apply(&WButton::SetIsEnabled, [this]() { return GetScene()->GetTimeScale() > 0.5f; })
			| Apply(&WButton::SetOnClicked, [this]() { 
				auto const scale{ GetScene()->GetTimeScale() };
				GetScene()->SetTimeScale(scale - 0.1f);
			})
		)
		| (
			UCreate<WText>{ "Game Speed Text" }()
			| Apply(&WText::SetText, [this]() { 
				return std::format("x{0:.1f}", GetScene()->GetTimeScale());
			})
		)
		| (
			UCreate<WButton>{ "Speed Down Button" }()
			| Apply(&WButton::SetIsEnabled, [this]() { return GetScene()->GetTimeScale() < 2.0f; })
			| Apply(&WButton::SetOnClicked, [this]() { 
				auto const scale{ GetScene()->GetTimeScale() };
				GetScene()->SetTimeScale(scale + 0.1f);
			})
		)
	);
}

#include "generated/GameSpeedButton.generated.inl"
