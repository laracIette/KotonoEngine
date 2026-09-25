#include "DirectionalLightComponent.h"

#include <kotono_common/enum_utils.h>
#include <kotono_graphics/SceneRenderGraph.h>
#include <kotono_graphics/SceneVisibility.h>

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
}

void KDirectionalLightComponent::PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph, ESceneVisibility visibility) const
{
	if (has_flag(visibility, ESceneVisibility::DirectionalLight))
	{
		sceneRenderGraph.directionalLightDatas.push_back({
			.direction = ForwardVector(),
			.color = color_,
			.intensity = intensity_,
			.castShadow = true,
		});
	}
}

#include "generated/DirectionalLightComponent.generated.inl"
