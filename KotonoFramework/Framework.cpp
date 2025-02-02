#include "Framework.h"

KtFramework::KtFramework() :
    _window(),
    _audioManager(),
    _inputManager()
{
}

void KtFramework::Run()
{
    _window.Init();
    _audioManager.Init();
    _inputManager.Init();

    _window.MainLoop();

    _audioManager.Cleanup();
    _window.Cleanup();
}

KtWindow& KtFramework::GetWindow()
{
    return _window;
}

KtAudioManager& KtFramework::GetAudioManager()
{
    return _audioManager;
}

KtInputManager& KtFramework::GetInputManager()
{
    return _inputManager;
}