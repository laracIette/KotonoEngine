#pragma once
#include "generated/AudioComponent.generated.h"
#include <kotono_core/SceneComponent.h>

#include <kotono_common/Handle.h>
class KAudioComponent final : public KSceneComponent
{
	GENERATED_KAUDIOCOMPONENT()

public:
	KAudioComponent();
	~KAudioComponent() override;

	void Spawn() override;
	void Despawn() override;

private:
	EHandle audioSourceHandle_;
	SERIALIZE WritableProperty(UPath, audioSource_, AudioSource);
};