#include "PointLightComponent/PointLightComponent.h"

#include <enum_utils.h>
#include <RenderGraph/SceneRenderGraph.h>
#include <SceneVisibility.h>

KPointLightComponent::KPointLightComponent()
	: range_{ 3.0f }
	, color_{ Colors::White }
	, intensity_{ 100.0f }
{
}

void KPointLightComponent::Spawn()
{
	Base::Spawn();
}

void KPointLightComponent::PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph, ESceneVisibility visibility) const
{
	if (has_flag(visibility, ESceneVisibility::PointLight))
	{
		sceneRenderGraph.pointLightDatas.push_back({
			.position = GetWorldPosition(),
			.range = range_,
			.color = color_,
			.intensity = intensity_,
		});
	}
}

#include "generated/PointLightComponent.generated.inl"
