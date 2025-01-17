#pragma once
#include <AL/alc.h>
class AudioManager final
{
public:
	void Init();
	void Cleanup();

private:
	ALCdevice* _device;
	ALCcontext* _context;
};

