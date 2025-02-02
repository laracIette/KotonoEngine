#pragma once
#include "Window.h"
#include "AudioManager.h"
#include "InputManager.h"
class KotonoFramework final
{
public:
	KotonoFramework();

	void Run();

	KtWindow& GetWindow();
	KtAudioManager& GetAudioManager();
	KtInputManager& GetInputManager();

	const std::string& GetProjectDirectory() const;

private:
	KtWindow _window;
	KtAudioManager _audioManager;
	KtInputManager _inputManager;

	std::string _projectDirectory;
};

extern KotonoFramework Framework;