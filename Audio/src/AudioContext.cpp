#include "AudioContext.h"

#include "al_utils.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <stdexcept>

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
        alListenerf(AL_GAIN, 1.0f),
        "couldn't set the listener's gain"
    );
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

auto UAudioContext::CreateSource(UPath const& path) -> u32
{
    if (!freeSourceSlots_.empty())
    {
        u32 const handle{ freeSourceSlots_.back() };
        freeSourceSlots_.pop_back();
        sources_.emplace(sources_.begin() + handle, path);
        return handle;
    }
    
    sources_.emplace_back(path);
    return static_cast<u32>(sources_.size() - 1);
}

void UAudioContext::DeleteSource(u32 handle)
{
    sources_[handle].Stop();
    freeSourceSlots_.push_back(handle);
}

void UAudioContext::SetSourceState(u32 handle, EAudioSourceState state)
{
    sources_[handle].SetState(state);
}

void UAudioContext::PlaySource(UPath const& path, b8 isLooping)
{
    oneTimeSources_.emplace_back(path);
    oneTimeSources_.back().SetIsLooping(isLooping);
}
