#include "PointLightComponent.h"

#include <kotono_graphics/SceneRenderGraph.h>

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

void KPointLightComponent::PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const
{
	sceneRenderGraph.pointLightDatas.push_back({
		.position = GetWorldPosition(),
		.range = range_,
		.color = color_,
		.intensity = intensity_,
	});
}

#include "generated/PointLightComponent.generated.inl"
