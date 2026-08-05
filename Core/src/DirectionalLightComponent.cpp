#include "DirectionalLightComponent.h"
#include <kotono_graphics/RenderContext.h>

KDirectionalLightComponent::KDirectionalLightComponent()
	: color_{ Colors::White }
	, intensity_{ 1.0f }
{
}

KDirectionalLightComponent::~KDirectionalLightComponent()
{
}

void KDirectionalLightComponent::Spawn()
{
	Base::Spawn();

	GetRenderContext().GetRenderer().RegisterDirectionalLight({
		.direction = ForwardVector(),
		.color = color_,
		.intensity = intensity_,
		.castShadow = true,
	});
}

void KDirectionalLightComponent::SetColor(const UColor& color)
{
	color_ = color;
}

void KDirectionalLightComponent::SetIntensity(const f32 intensity)
{
	intensity_ = intensity;
}

#include "generated/DirectionalLightComponent.generated.inl"
