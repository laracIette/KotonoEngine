#pragma once
#include "Window.h"
#include "AudioManager.h"
#include "InputManager.h"
class KtFramework final
{
public:
	KtFramework();

	void Run();

	KtWindow& GetWindow();
	KtAudioManager& GetAudioManager();
	KtInputManager& GetInputManager();

private:
	KtWindow _window;
	KtAudioManager _audioManager;
	KtInputManager _inputManager;
};

extern KtFramework Framework;

static VkDevice getVulkanDevice()
{
	return Framework.GetWindow().GetContext().GetDevice();
}