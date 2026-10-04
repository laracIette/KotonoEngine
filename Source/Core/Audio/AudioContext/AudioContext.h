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
	struct AudioSourceCreateInfo
	{
		f32 volume{ 1.0f };
		f32 pitch{ 1.0f };
		f32 attenuationFactor{ 1.0f };
		f32 attenuationStartDistance{ 1.0f };
		f32 attenuationEndDistance{ 10.0f };
		b8 isLooping{ false };
	};

public:
	void Init();
	void Cleanup();

	void Update();

	auto CreateSource(UPath const& path) -> EHandle;
	auto GetSource(EHandle handle) -> UAudioSource&;
	void DeleteSource(EHandle handle);

	/// One time play and delete interface
	void PlaySource(UPath const& path, b8 isLooping = false);
	/// One time play and delete scene
	void PlaySource(UPath const& path, glm::vec3 const& position, AudioSourceCreateInfo const& createInfo = {});

	void SetListenerPosition(glm::vec3 const& position) const;
	void SetListenerOrientation(glm::quat const& orientation) const;

private:
	ALCdevice* device_;
	ALCcontext* context_;

	std::vector<UAudioSource> oneTimeSources_;

	std::vector<UAudioSource> sources_;
	std::vector<EHandle> freeSourceSlots_;
};
