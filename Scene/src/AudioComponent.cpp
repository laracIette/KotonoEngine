#include "AudioComponent.h"

#include <kotono_core/Scene.h>

KAudioComponent::KAudioComponent()
	: audioSourceHandle_{ EHandle::Invalid }
{
}

KAudioComponent::~KAudioComponent()
{
}

void KAudioComponent::Spawn()
{
	Base::Spawn();

	if (audioSource_)
	{
		audioSourceHandle_ = GetScene()->GetAudioContext().CreateSource(audioSource_);
		auto& source{ GetScene()->GetAudioContext().GetSource(audioSourceHandle_) };
		source.SetIsLooping(true);
		source.SetState(EAudioSourceState::Playing);
		source.SetPosition(GetWorldPosition());
		source.SetSpace(EAudioSourceSpace::Scene);
		source.SetVolume(1.0f);
		source.SetAttenuationFactor(1.0f);
		source.SetAttenuationStartDistance(5.0f);
		source.SetAttenuationEndDistance(10.0f);
	}
}

void KAudioComponent::Despawn()
{
	Base::Despawn();

	if (audioSourceHandle_ != EHandle::Invalid)
	{
		GetScene()->GetAudioContext().DeleteSource(audioSourceHandle_);
	}
}

#include "generated/AudioComponent.generated.inl"
