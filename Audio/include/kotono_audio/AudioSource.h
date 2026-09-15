#pragma once
#include <dr_libs/dr_wav.h>
#include <kotono_common/Path.h>

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

private:
	struct DataWAV
	{
		DataWAV(UPath const& path);
		~DataWAV();

		drwav_int16* pSampleData;
		drwav_uint64 totalFrameCount;
		u32 channels;
		u32 sampleRate;
		size dataSize;
	};

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