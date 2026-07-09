#include "DirectionalLightComponent.h"
#include <kotono_graphics/Camera.h>
#include <kotono_graphics/LightBuffers.h>
#include <kotono_platform/glm_utils.h>

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
	const auto direction{ ForwardVector() };

	LightBuffers.RegisterDirectionalLight({
		.direction = direction,
		.color = color_,
		.intensity = intensity_,
		.castShadow = true,
		.lightViewProj = get_light_space_matrix(SCamera::GetProjectionMatrix(), SCamera::GetViewMatrix(), direction),
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
