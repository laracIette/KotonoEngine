#include "AudioManager.h"
#include <AL/alc.h>

void KtAudioManager::Init()
{
    device_ = alcOpenDevice(nullptr); // Select the default device
    if (!device_)
    {
        throw "Failed to open OpenAL device";
    }

    context_ = alcCreateContext(device_, nullptr);
    if (!context_)
    {
        throw "Failed to create OpenAL context";
    }

    alcMakeContextCurrent(context_);
}

void KtAudioManager::Cleanup()
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context_);
    alcCloseDevice(device_);
}