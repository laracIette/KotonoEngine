#pragma once
#include "AudioSource.h"
#include <kotono_common/Handle.h>
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

	auto CreateSource(UPath const& path) -> EHandle;
	auto GetSource(EHandle handle) -> UAudioSource&;
	void DeleteSource(EHandle handle);

	/// One time play and delete
	void PlaySource(UPath const& path, b8 isLooping = false);

	void SetListenerPosition(glm::vec3 const& position) const;
	void SetListenerOrientation(glm::quat const& orientation) const;

private:
	ALCdevice* device_;
	ALCcontext* context_;

	std::vector<UAudioSource> oneTimeSources_;

	std::vector<UAudioSource> sources_;
	std::vector<EHandle> freeSourceSlots_;
};
