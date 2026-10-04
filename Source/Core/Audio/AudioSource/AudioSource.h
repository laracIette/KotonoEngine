#pragma once
#include <glm/fwd.hpp>
#include <kotono_common/Clamped.h>
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
enum class EAudioSourceState : u8
{
	Initial,
	Playing,
	Paused,
	Stopped
};
enum class EAudioSourceSpace : u8
{
	Interface,
	Scene
};
class UAudioSource final
{
public:
	using VolumeRange = UClamped<f32, 0.0f, 1.0f>;
	using PitchRange = UClamped<f32, 0.5f, 2.0f>;
	using AttenuationFactorRange = UClamped<f32, 0.0f, 1.0f>;
	using AttenuationDistanceRange = UClamped<f32, 0.0f, std::numeric_limits<f32>::max()>;

public:
	UAudioSource(UPath const& path);
	~UAudioSource();

	auto GetState() const -> EAudioSourceState;
	auto GetVolume() const -> f32;
	auto GetPitch() const -> f32;
	auto GetIsLooping() const -> b8;
	auto GetPosition() const -> glm::vec3;
	auto GetSpace() const -> EAudioSourceSpace;
	auto GetAttenuationFactor() const -> f32;
	auto GetAttenuationStartDistance() const -> f32;
	auto GetAttenuationEndDistance() const -> f32;

	void SetState(EAudioSourceState state) const;
	void SetVolume(VolumeRange volume) const;
	void SetPitch(PitchRange pitch) const;
	void SetIsLooping(b8 isLooping) const;
	void SetPosition(glm::vec3 const& position) const;
	void SetSpace(EAudioSourceSpace space) const;
	void SetAttenuationFactor(AttenuationFactorRange factor) const;
	void SetAttenuationStartDistance(AttenuationDistanceRange distance) const;
	void SetAttenuationEndDistance(AttenuationDistanceRange distance) const;

	/// Sets state to State::Playing
	void Play() const;
	/// Sets state to State::Paused
	void Pause() const;
	/// Sets state to State::Initial
	void Rewind() const;
	/// Sets state to State::Stopped
	void Stop() const;

private:
	u32 source_;
	u32 buffer_;
};