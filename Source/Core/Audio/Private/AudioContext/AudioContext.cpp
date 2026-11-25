#include "AudioContext/AudioContext.h"

#include "al_utils/al_utils.h"
#include "check.h"
#include "Containers/Array.h"
#include "Containers/Span.h"
#include "enum_utils.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>
#include <stdexcept>

static void getListener(ALenum param, f32& value) { alGetListenerf(param, &value); }
static void getListener(ALenum param, i32& value) { alGetListeneri(param, &value); }
static void getListener(ALenum param, glm::vec3& value) { alGetListener3f(param, &value.x, &value.y, &value.z); }

static void setListener(ALenum param, std::floating_point auto value) { alListenerf(param, static_cast<f32>(value)); }
static void setListener(ALenum param, std::integral auto value) { alListeneri(param, static_cast<i32>(value)); }
static void setListener(ALenum param, glm::vec3 const& value) { alListener3f(param, value.x, value.y, value.z); }
static void setListener(ALenum param, USpan<f32 const> values) { alListenerfv(param, values.Data()); }

template <typename T>
static auto GetListener(ALenum param) -> T
{
    T value;
    AL_CHECK_THROW(
        getListener(param, value),
        "couldn't get the listener's param!"
    );
    return value;
}

template <typename T>
static void SetListener(ALenum param, T value)
{
    AL_CHECK_THROW(
        setListener(param, value),
        "couldn't set the listener's param!"
    );
}

UAudioContext::UAudioContext()
	: device_{ nullptr }
	, context_{ nullptr }
	, currentHandle_{ 0 }
{
}

void UAudioContext::Init()
{
	device_ = alcOpenDevice(nullptr);
	if (!device_)
	{
		throw std::runtime_error{ "Failed to open OpenAL device" };
	}

	context_ = alcCreateContext(device_, nullptr);
	if (!context_)
	{
		throw std::runtime_error{ "Failed to create OpenAL context" };
	}

	alcMakeContextCurrent(context_);

	AL_CHECK_THROW(
		alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED),
		"couldn't set the context's distance model!"
	);

	setListener(AL_GAIN, 1.0f);
}

void UAudioContext::Cleanup()
{
	sources_.Clear();
	oneTimeSources_.Clear();

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context_);
	alcCloseDevice(device_);
}

void UAudioContext::Update()
{
	for (auto const& source : oneTimeSources_)
	{
		if (source.GetState() == EAudioSourceState::Initial)
		{
			source.Play();
		}
	}

    oneTimeSources_ = oneTimeSources_
        | std::views::filter([](UAudioSource const& source) {
		    return source.GetState() != EAudioSourceState::Stopped;
	    })
        | std::ranges::to<UList>();
}

auto UAudioContext::CreateSource(UPath const& path) -> EHandle
{
	if (!freeSourceSlots_.IsEmpty())
	{
		EHandle const handle{ freeSourceSlots_.Last() };
		freeSourceSlots_.Pop();
		sources_.TryEmplace(handle, path);
		return handle;
	}
    
	sources_.TryEmplace(currentHandle_, path);
	return currentHandle_++;
}

auto UAudioContext::GetSource(EHandle handle) -> UAudioSource&
{
	Check(Abort, handle != EHandle::Invalid, "handle is invalid");
	return sources_.At(handle);
}

void UAudioContext::DeleteSource(EHandle handle)
{
	Check(Throw, handle != EHandle::Invalid, "handle is invalid");
	
	sources_.Remove(handle);
	freeSourceSlots_.Add(handle);
}

void UAudioContext::PlaySource(UPath const& path, b8 isLooping)
{
	oneTimeSources_.Add(UAudioSource{ path });
	oneTimeSources_.Last().SetSpace(EAudioSourceSpace::Interface);
	oneTimeSources_.Last().SetIsLooping(isLooping);
}

void UAudioContext::PlaySource(UPath const& path, glm::vec3 const& position, AudioSourceCreateInfo const& createInfo)
{
	oneTimeSources_.Add(UAudioSource{ path });
	oneTimeSources_.Last().SetSpace(EAudioSourceSpace::Scene);
	oneTimeSources_.Last().SetPosition(position);
	oneTimeSources_.Last().SetVolume(createInfo.volume);
	oneTimeSources_.Last().SetPitch(createInfo.pitch);
	oneTimeSources_.Last().SetAttenuationFactor(createInfo.attenuationFactor);
	oneTimeSources_.Last().SetAttenuationStartDistance(createInfo.attenuationStartDistance);
	oneTimeSources_.Last().SetAttenuationEndDistance(createInfo.attenuationEndDistance);
	oneTimeSources_.Last().SetIsLooping(createInfo.isLooping);
}

void UAudioContext::SetListenerPosition(glm::vec3 const& position) const
{
	SetListener(AL_POSITION, position * glm::vec3{ 1.0f, 1.0f, -1.0f });
}

void UAudioContext::SetListenerOrientation(glm::quat const& orientation) const
{
	glm::vec3 const forward{ orientation * glm::vec3{ 0.0f, 0.0f, -1.0f } };
	glm::vec3 const up{ orientation * glm::vec3{ 0.0f, 1.0f, 0.0f } };

	UArray const values{
		forward.x, forward.y, forward.z,
		up.x, up.y, up.z
	};
	SetListener(AL_ORIENTATION, values);
}
