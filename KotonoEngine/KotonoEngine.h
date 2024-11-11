#pragma once
#include "AudioManager.h"
#include "Window.h"
class KotonoEngine final
{
public:
	KotonoEngine();
	~KotonoEngine();

	void Run();

	Window* GetWindow() const;
	AudioManager* GetAudioManager() const;

private:
	Window* _window;
	AudioManager* _audioManager;
};

extern KotonoEngine* Engine;

