#pragma once
#include <SceneComponent/SceneComponent.h>

#include <Color.h>

#include "PointLightComponent.generated.h"

class KPointLightComponent : public KSceneComponent
{
	GENERATED()

public:
	KPointLightComponent();

	void Spawn() override;

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph, ESceneVisibility visibility) const override;

private:
	SERIALIZE WritableProperty(f32, range_, Range);
	SERIALIZE WritableProperty(UColor, color_, Color);
	SERIALIZE WritableProperty(f32, intensity_, Intensity);
};