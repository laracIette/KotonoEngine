#pragma once
#include "Window.h"
#include "AudioManager.h"
#include "InputManager.h"
class KotonoEngine final
{
public:
	KotonoEngine();

	void Run();

	Window& GetWindow();
	AudioManager& GetAudioManager();
	InputManager& GetInputManager();

	const std::string& GetProjectDirectory() const;

private:
	Window _window;
	AudioManager _audioManager;
	InputManager _inputManager;

	std::string _projectDirectory;
};

extern KotonoEngine Engine;

