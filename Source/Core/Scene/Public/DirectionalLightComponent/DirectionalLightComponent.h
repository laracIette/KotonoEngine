#pragma once
#include <SceneComponent/SceneComponent.h>

#include <Color.h>

#include "DirectionalLightComponent.generated.h"

class KDirectionalLightComponent : public KSceneComponent
{
	GENERATED()

public:
	KDirectionalLightComponent();
	~KDirectionalLightComponent() override;

	void Spawn() override;

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph, ESceneVisibility visibility) const override;

private:
	SERIALIZE WritableProperty(UColor, color_, Color);
	SERIALIZE WritableProperty(f32, intensity_, Intensity);
};