#pragma once

struct ALCdevice;
struct ALCcontext;

class KtAudioManager final
{
	friend class SEngine;

private:
	void Init();
	void Cleanup();

private:
	ALCdevice* device_;
	ALCcontext* context_;
};

inline KtAudioManager AudioManager;
