#pragma once
#include "generated/DirectionalLightComponent.generated.h"
#include <kotono_core/SceneComponent.h>
#include <kotono_graphics/Color.h>
class KDirectionalLightComponent : public KSceneComponent
{
	GENERATED_KDIRECTIONALLIGHTCOMPONENT()

public:
	KDirectionalLightComponent();
	~KDirectionalLightComponent() override;

	void Spawn() override;

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const override;

private:
	SERIALIZE WritableProperty(UColor, color_, Color);
	SERIALIZE WritableProperty(f32, intensity_, Intensity);
};