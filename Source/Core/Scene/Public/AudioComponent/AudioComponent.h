#pragma once
#include <SceneComponent/SceneComponent.h>

#include <Handle.h>

#include "AudioComponent.generated.h"

class KAudioComponent final : public KSceneComponent
{
	GENERATED()

public:
	KAudioComponent();
	~KAudioComponent() override;

	void Spawn() override;
	void Despawn() override;

private:
	EHandle audioSourceHandle_;
	SERIALIZE WritableProperty(UPath, audioSource_, AudioSource);
};