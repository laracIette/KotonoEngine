#pragma once
#include "generated/DirectionalLightComponent.generated.h"
#include "SceneComponent.h"
#include <kotono_graphics/Color.h>
class KDirectionalLightComponent : public KSceneComponent
{
	GENERATED_KDIRECTIONALLIGHTCOMPONENT()

public:
	KDirectionalLightComponent();
	~KDirectionalLightComponent();

	void Spawn() override;

	void SetColor(const UColor& color);
	void SetIntensity(const f32 intensity);

private:
	SERIALIZE ReadonlyProperty(UColor, color_, Color);
	SERIALIZE ReadonlyProperty(f32, intensity_, Intensity);
	u32 shadowMap_;
};