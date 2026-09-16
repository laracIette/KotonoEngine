#pragma once
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
enum class EAudioSourceState : u8
{
	Initial,
	Playing,
	Paused,
	Stopped
};
class UAudioSource final
{
public:
	UAudioSource(UPath const& path);
	~UAudioSource();

	auto GetState() const -> EAudioSourceState;
	void SetState(EAudioSourceState state) const;

	auto GetVolume() const -> f32;
	void SetVolume(f32 volume) const;

	/// Sets state to State::Playing
	void Play() const;
	/// Sets state to State::Paused
	void Pause() const;
	/// Sets state to State::Initial
	void Rewind() const;
	/// Sets state to State::Stopped
	void Stop() const;

	void SetIsLooping(b8 isLooping) const;

private:
	u32 source_;
	u32 buffer_;
};