#include "Framework.h"

void KtFramework::Cleanup()
{
    _inputManager.Cleanup();
    _audioManager.Cleanup();
    _renderer.Cleanup();
    _context.Cleanup();
    _window.Cleanup();
}

void KtFramework::Init()
{
    _window.Init();
    _context.Init();
    _renderer.Init();
    _audioManager.Init();
    _inputManager.Init();
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