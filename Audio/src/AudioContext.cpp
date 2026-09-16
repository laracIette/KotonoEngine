#include "AudioContext.h"

#include "al_utils.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <array>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>
#include <span>
#include <stdexcept>

static void _getListener(ALenum param, f32& value) { alGetListenerf(param, &value); }
static void _getListener(ALenum param, i32& value) { alGetListeneri(param, &value); }
static void _getListener(ALenum param, glm::vec3& value) { alGetListener3f(param, &value.x, &value.y, &value.z); }

static void _setListener(ALenum param, std::floating_point auto value) { alListenerf(param, static_cast<f32>(value)); }
static void _setListener(ALenum param, std::integral auto value) { alListeneri(param, static_cast<i32>(value)); }
static void _setListener(ALenum param, glm::vec3 const& value) { alListener3f(param, value.x, value.y, value.z); }
static void _setListener(ALenum param, std::span<f32 const> values) { alListenerfv(param, values.data()); }

template <typename T>
static auto getListener(ALenum param) -> T
{
    T value;
    AL_CHECK_THROW(
        _getListener(param, value),
        "couldn't get the listener's param!"
    );
    return value;
}

template <typename T>
static void setListener(ALenum param, T value)
{
    AL_CHECK_THROW(
        _setListener(param, value),
        "couldn't set the listener's param!"
    );
}

void UAudioContext::Init()
{
    device_ = alcOpenDevice(nullptr); // Select the default device
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
    sources_.clear();
    oneTimeSources_.clear();

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

    std::erase_if(oneTimeSources_, [](UAudioSource const& source) { 
        return source.GetState() == EAudioSourceState::Stopped; 
    });
}

auto UAudioContext::CreateSource(UPath const& path) -> EHandle
{
    if (!freeSourceSlots_.empty())
    {
        EHandle const handle{ freeSourceSlots_.back() };
        freeSourceSlots_.pop_back();
        sources_.emplace(sources_.begin() + static_cast<size>(handle), path);
        return handle;
    }
    
    sources_.emplace_back(path);
    return static_cast<EHandle>(sources_.size() - 1);
}

void UAudioContext::DeleteSource(EHandle handle)
{
    sources_[static_cast<size>(handle)].Stop();
    freeSourceSlots_.push_back(handle);
}

auto UAudioContext::GetSource(EHandle handle) -> UAudioSource&
{
    return sources_[static_cast<size>(handle)];
}

void UAudioContext::PlaySource(UPath const& path, b8 isLooping)
{
    oneTimeSources_.emplace_back(path);
    oneTimeSources_.back().SetIsLooping(isLooping);
}

void UAudioContext::SetListenerPosition(glm::vec3 const& position) const
{
    setListener(AL_POSITION, position * glm::vec3{ 1.0f, 1.0f, -1.0f });
}

void UAudioContext::SetListenerOrientation(glm::quat const& orientation) const
{
    glm::vec3 const forward{ orientation * glm::vec3{ 0.0f, 0.0f, -1.0f } };
    glm::vec3 const up{ orientation * glm::vec3{ 0.0f, 1.0f, 0.0f } };

    std::array const values{
        forward.x, forward.y, forward.z,
        up.x, up.y, up.z
    };
    setListener(AL_ORIENTATION, values);
}
