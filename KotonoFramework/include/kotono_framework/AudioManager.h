#pragma once

struct ALCdevice;
struct ALCcontext;

class KtAudioManager final
{
	friend class KtFramework;

private:
	void Init();
	void Cleanup();

private:
	ALCdevice* device_;
	ALCcontext* context_;
};

