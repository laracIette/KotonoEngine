#include "PointLightComponent.h"

#include <kotono_common/enum_utils.h>
#include <kotono_graphics/SceneRenderGraph.h>
#include <kotono_graphics/SceneVisibility.h>

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
