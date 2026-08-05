#pragma once

struct ALCdevice;
struct ALCcontext;

class GAudioManager final
{
public:
	void Init();
	void Cleanup();

private:
	ALCdevice* device_;
	ALCcontext* context_;
};

inline GAudioManager AudioManager;
