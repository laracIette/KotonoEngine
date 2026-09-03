#pragma once
#include "generated/PointLightComponent.generated.h"
#include <kotono_core/SceneComponent.h>
#include <kotono_graphics/Color.h>
class KPointLightComponent : public KSceneComponent
{
	GENERATED_KPOINTLIGHTCOMPONENT()

public:
	KPointLightComponent();

	void Spawn() override;

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const override;

private:
	SERIALIZE WritableProperty(f32, range_, Range);
	SERIALIZE WritableProperty(UColor, color_, Color);
	SERIALIZE WritableProperty(f32, intensity_, Intensity);
};