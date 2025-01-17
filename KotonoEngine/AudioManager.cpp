#include "AudioManager.h"

void AudioManager::Init()
{
    _device = alcOpenDevice(nullptr); // Select the default device
    if (!_device)
    {
        throw "Failed to open OpenAL device";
    }

    _context = alcCreateContext(_device, nullptr);
    if (!_context)
    {
        throw "Failed to create OpenAL context";
    }

    alcMakeContextCurrent(_context);
}

void AudioManager::Cleanup()
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(_context);
    alcCloseDevice(_device);
}