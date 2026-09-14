#include "GameSpeedButton.h"

#include <kotono_core/Scene.h>
#include <kotono_interface/widgets.h>

WidgetPtr WGameSpeedButton::Build()
{
	UPtr speedText{ UCreate<WText>{ "Game Speed Text" }("Game Speed Text") };
	speedText->SetText([]() { return ""; });

	auto const widgetTree{ UChildrenOwnerTree{ UCreate<WRow>{}(), {
		new UWidgetTreeLeaf{ speedText },
	} } };

	widgetTree.Link();

	return widgetTree.Widget();
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
