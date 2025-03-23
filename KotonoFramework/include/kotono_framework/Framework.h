#pragma once
#include "Window.h"
#include "Context.h"
#include "Renderer.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "ImageTextureManager.h"
class KtFramework final
{
public:
	void Init();
	void Cleanup();

	KtWindow& GetWindow();
	KtContext& GetContext();
	KtRenderer& GetRenderer();
	KtAudioManager& GetAudioManager();
	KtInputManager& GetInputManager();
	KtImageTextureManager& GetImageTextureManager();

private:
	KtWindow _window;
	KtContext _context;
	KtRenderer _renderer;
	KtAudioManager _audioManager;
	KtInputManager _inputManager;
	KtImageTextureManager _imageTextureManager;
};

extern KtFramework Framework;
