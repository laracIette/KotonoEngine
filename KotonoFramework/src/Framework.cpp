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

    auto& event = _inputManager.GetKeyboard().GetEvent(KT_KEY_SPACE);
    event.AddListener(&_window, &KtWindow::OnKeySpacePressed);

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