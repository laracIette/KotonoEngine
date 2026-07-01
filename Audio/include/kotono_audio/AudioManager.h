#pragma once

struct ALCdevice;
struct ALCcontext;

class GAudioManager final
{
	friend class GCore;

private:
	void Init();
	void Cleanup();

private:
	ALCdevice* device_;
	ALCcontext* context_;
};

inline GAudioManager AudioManager;
