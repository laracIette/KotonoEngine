#pragma once
#include <AL/alc.h>
class AudioManager final
{
public:
	AudioManager();
	~AudioManager();

	void Init();

private:
	ALCdevice* _device;
	ALCcontext* _context;
};

