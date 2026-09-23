#include "GameSpeedButton.h"

#include <kotono_core/Scene.h>
#include <kotono_interface/widgets.h>

WidgetPtr WGameSpeedButton::Build()
{
	return (
		UCreate<WRow>{}()
		| (
			UCreate<WText>{ "Game Speed Text" }()
			| Apply(&WText::SetText, []() { return "unimplemented!"; })
		)
	);
}

void WGameSpeedButton::Display(UWidgetDisplaySettings const& displaySettings)
{
	Base::Display(displaySettings);

	GetScene()->GetEventTimeScaleChanged().AddListener(this, &Self::OnTimeScaleChanged);
}

void WGameSpeedButton::Remove()
{
	Base::Remove();

	GetScene()->GetEventTimeScaleChanged().RemoveListener(this, &Self::OnTimeScaleChanged);
}

void WGameSpeedButton::OnTimeScaleChanged(f32 timeScale) const
{
}

#include "generated/GameSpeedButton.generated.inl"
