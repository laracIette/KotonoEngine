#pragma once
#include "Path.h"
#include "Window.h"
#include "Context.h"
#include "Renderer.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "ImageTextureManager.h"
class KtFramework final
{
public:
	void Init();
	void Cleanup();

	KtPath& GetPath();
	KtWindow& GetWindow();
	KtContext& GetContext();
	KtRenderer& GetRenderer();
	KtAudioManager& GetAudioManager();
	KtInputManager& GetInputManager();
	KtModelManager& GetModelManager();
	KtImageTextureManager& GetImageTextureManager();

private:
	KtPath _path;
	KtWindow _window;
	KtContext _context;
	KtRenderer _renderer;
	KtAudioManager _audioManager;
	KtInputManager _inputManager;
	KtModelManager _modelManager;
	KtImageTextureManager _imageTextureManager;
};

extern KtFramework Framework;
