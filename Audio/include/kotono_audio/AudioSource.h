#pragma once
#include <glm/fwd.hpp>
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
	void SetVolume(f32 volume) const;
	void SetPitch(f32 pitch) const;
	void SetIsLooping(b8 isLooping) const;
	void SetPosition(glm::vec3 const& position) const;
	void SetSpace(EAudioSourceSpace space) const;
	void SetAttenuationFactor(f32 factor) const;
	void SetAttenuationStartDistance(f32 distance) const;
	void SetAttenuationEndDistance(f32 distance) const;

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