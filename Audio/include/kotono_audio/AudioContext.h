#pragma once
#include "AudioSource.h"
#include <kotono_common/Path.h>
#include <vector>
enum class EAudioSourceState : u8;
struct ALCdevice;
struct ALCcontext;
class UAudioSource;
class UAudioContext final
{
public:
	void Init();
	void Cleanup();

	void Update();

	auto CreateSource(UPath const& path) -> u32;
	void DeleteSource(u32 handle);
	void SetSourceState(u32 handle, EAudioSourceState state);

	/// One time play and delete
	void PlaySource(UPath const& path, b8 isLooping = false);

private:
	ALCdevice* device_;
	ALCcontext* context_;

	std::vector<UAudioSource> oneTimeSources_;

	std::vector<UAudioSource> sources_;
	std::vector<u32> freeSourceSlots_;
};
