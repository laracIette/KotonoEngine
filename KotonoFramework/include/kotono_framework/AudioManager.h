#pragma once

struct ALCdevice;
struct ALCcontext;

class KtAudioManager final
{
public:
	void Init();
	void Cleanup();

private:
	ALCdevice* device_;
	ALCcontext* context_;
};

