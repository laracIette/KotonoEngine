#include "PointLightComponent.h"
#include <kotono_graphics/Renderer.h>

KPointLightComponent::KPointLightComponent()
	: range_{ 3.0f }
	, color_{ Colors::White }
	, intensity_{ 100.0f }
{
}

void KPointLightComponent::Spawn()
{
	Base::Spawn();

	Renderer.RegisterPointLight({
		.position = GetWorldPosition(),
		.range = range_,
		.color = color_,
		.intensity = intensity_,
	});
}

void KPointLightComponent::SetRange(const f32 range)
{
	range_ = range;
}

void KPointLightComponent::SetColor(const UColor& color)
{
	color_ = color;
}

void KPointLightComponent::SetIntensity(const f32 intensity)
{
	intensity_ = intensity;
}

#include "generated/PointLightComponent.generated.inl"
