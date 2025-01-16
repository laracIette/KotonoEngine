#pragma once
#include "Window.h"
#include "AudioManager.h"
#include "InputManager.h"
class KotonoEngine final
{
public:
	KotonoEngine();
	~KotonoEngine();

	void Run();

	Window* GetWindow() const;
	AudioManager* GetAudioManager() const;
	InputManager* GetInputManager() const;

	const std::string& GetProjectDirectory() const;

private:
	Window* _window;
	AudioManager* _audioManager;
	InputManager* _inputManager;

	std::string _projectDirectory;
};

extern KotonoEngine* Engine;

