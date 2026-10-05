#include "DirectionalLightComponent/DirectionalLightComponent.h"

#include <enum_utils.h>
#include <RenderGraph/SceneRenderGraph.h>
#include <SceneVisibility.h>

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

#include "DirectionalLightComponent.generated.inl"
