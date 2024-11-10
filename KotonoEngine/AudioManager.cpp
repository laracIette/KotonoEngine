#include "AudioManager.h"

AudioManager::AudioManager() :
    _device(nullptr), _context(nullptr)
{
}

AudioManager::~AudioManager()
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(_context);
    alcCloseDevice(_device);
}

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
