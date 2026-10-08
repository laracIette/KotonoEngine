#include "GameSpeedButton.h"

#include <Scene/Scene.h>
#include <ValueSlider/ValueSlider.h>
#include <core_widgets.h>

WidgetPtr WGameSpeedButton::Build()
{
	return (
		UCreate<WRow>{}()
		| (
			UCreate<WValueSlider>{}()
			| Apply(&WValueSlider::SetValueToString, [this]() { return to_string<f32>(GetScene()->GetTimeScale()); })
			| Apply(&WValueSlider::SetOnTextChanged, [this](std::string_view text) { GetScene()->SetTimeScale(from_string<f32>(text)); })
			| Apply(&WValueSlider::SetOnSlide, [this](f32 delta) {
				auto scale{ GetScene()->GetTimeScale() };
				scale += delta * std::max(0.01f, std::abs(scale * 0.01f));
				GetScene()->SetTimeScale(scale);
			})
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
