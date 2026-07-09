#include "DirectionalLightComponent.h"
#include <glm/gtx/quaternion.hpp>
#include <kotono_common/AssetManager.h>
#include <kotono_graphics/Camera.h>
#include <kotono_graphics/LightBuffers.h>
#include <kotono_graphics/PipelineResourceManager.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/Sampler.h>
#include <kotono_platform/glm_utils.h>

KDirectionalLightComponent::KDirectionalLightComponent()
	: color_{ Colors::White }
	, intensity_{ 1.0f }
{
	const auto imageIndex{ Renderer.RegisterDirectionalShadowMapTarget() };
	const auto imageView{ Renderer.GetDirectionalShadowMapTargetImageView(imageIndex) };
	shadowMap_ = PipelineResourceManager.RegisterTexture(imageView);
}

KDirectionalLightComponent::~KDirectionalLightComponent()
{
	PipelineResourceManager.UnregisterTexture(shadowMap_);
}

void KDirectionalLightComponent::Spawn()
{
	Base::Spawn();

	if (UAsset sampler{ SAssetManager<USampler>::Get("${ENGINE_DIRECTORY}/Graphics/assets/samplers/shadow.kasset") })
	{
		const auto direction{ ForwardVector() };

		LightBuffers.RegisterDirectionalLight({
			.direction = direction,
			.color = color_,
			.intensity = intensity_,
			.shadowMap = shadowMap_,
			.shadowSampler = sampler->GetIndex(),
			.lightViewProj = get_light_space_matrix(SCamera::GetProjectionMatrix(), SCamera::GetViewMatrix(), direction),
		});
	}
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
