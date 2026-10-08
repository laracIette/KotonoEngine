#pragma once
#include "AudioSource/AudioSource.h"
#include <Handle.h>
#include <Containers/Map.h>
#include <Path/Path.h>
#include <vector>

struct ALCdevice;
struct ALCcontext;

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
	UAudioContext();
	
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

	UMap<EHandle, UAudioSource> sources_;
	EHandle currentHandle_;
	std::vector<EHandle> freeSourceSlots_;
};
