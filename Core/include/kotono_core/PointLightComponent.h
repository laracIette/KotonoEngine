#pragma once
#include "generated/PointLightComponent.generated.h"
#include "SceneComponent.h"
#include <kotono_graphics/Color.h>
class KPointLightComponent : public KSceneComponent
{
	GENERATED_KPOINTLIGHTCOMPONENT()

public:
	KPointLightComponent();

	void Spawn() override;

	void SetRange(const f32 range);
	void SetColor(const UColor& color);
	void SetIntensity(const f32 intensity);

private:
	SERIALIZE ReadonlyProperty(f32, range_, Range);
	SERIALIZE ReadonlyProperty(UColor, color_, Color);
	SERIALIZE ReadonlyProperty(f32, intensity_, Intensity);
};