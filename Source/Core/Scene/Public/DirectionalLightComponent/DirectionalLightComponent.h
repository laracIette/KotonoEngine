#pragma once
#include "generated/DirectionalLightComponent.generated.h"
#include <SceneComponent/SceneComponent.h>

#include <Color.h>

class KDirectionalLightComponent : public KSceneComponent
{
	GENERATED_KDIRECTIONALLIGHTCOMPONENT()

public:
	KDirectionalLightComponent();
	~KDirectionalLightComponent() override;

	void Spawn() override;

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph, ESceneVisibility visibility) const override;

private:
	SERIALIZE WritableProperty(UColor, color_, Color);
	SERIALIZE WritableProperty(f32, intensity_, Intensity);
};