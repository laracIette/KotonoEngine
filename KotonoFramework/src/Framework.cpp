#include "Framework.h"

void KtFramework::Cleanup()
{
    _imageTextureManager.Cleanup();
    _modelManager.Cleanup();
    _inputManager.Cleanup();
    _audioManager.Cleanup();
    _renderer.Cleanup();
    _context.Cleanup();
    _window.Cleanup();
    _path.Cleanup();
}

void KtFramework::Init()
{
    _path.Init();
    _window.Init();
    _context.Init();
    _renderer.Init();
    _audioManager.Init();
    _inputManager.Init();
    _modelManager.Init();
    _imageTextureManager.Init();
}

KtPath& KtFramework::GetPath()
{
    return _path;
}

KtWindow& KtFramework::GetWindow()
{
    return _window;
}

KtContext& KtFramework::GetContext()
{
    return _context;
}

KtRenderer& KtFramework::GetRenderer()
{
    return _renderer;
}

KtAudioManager& KtFramework::GetAudioManager()
{
    return _audioManager;
}

KtInputManager& KtFramework::GetInputManager()
{
    return _inputManager;
}

KtModelManager& KtFramework::GetModelManager()
{
    return _modelManager;
}

KtImageTextureManager& KtFramework::GetImageTextureManager()
{
    return _imageTextureManager;
}
