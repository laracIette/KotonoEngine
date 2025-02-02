#include "KotonoFramework.h"

KotonoFramework::KotonoFramework() :
    _window(),
    _audioManager(),
    _inputManager(),
    _projectDirectory(R"(C:\Users\nicos\Documents\Kotono Projects\Test)")
{
}

void KotonoFramework::Run()
{
    _window.Init();
    _audioManager.Init();
    _inputManager.Init();

    _window.MainLoop();

    _audioManager.Cleanup();
    _window.Cleanup();
}

KtWindow& KotonoFramework::GetWindow()
{
    return _window;
}

KtAudioManager& KotonoFramework::GetAudioManager()
{
    return _audioManager;
}

KtInputManager& KotonoFramework::GetInputManager()
{
    return _inputManager;
}

const std::string& KotonoFramework::GetProjectDirectory() const
{
    return _projectDirectory;
}