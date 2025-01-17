#include "KotonoEngine.h"

KotonoEngine::KotonoEngine() :
    _window(),
    _audioManager(),
    _inputManager(),
    _projectDirectory(R"(C:\Users\nicos\Documents\Kotono Projects\Test)")
{
}

void KotonoEngine::Run()
{
    Init();
    MainLoop();
    Cleanup();
}

void KotonoEngine::Init()
{
    _window.Init();
    _audioManager.Init();
    _inputManager.Init();
}

void KotonoEngine::MainLoop()
{
}

void KotonoEngine::Cleanup()
{
    _audioManager.Cleanup();
    _window.Cleanup();
}

Window& KotonoEngine::GetWindow()
{
    return _window;
}

AudioManager& KotonoEngine::GetAudioManager()
{
    return _audioManager;
}

InputManager& KotonoEngine::GetInputManager()
{
    return _inputManager;
}

const std::string& KotonoEngine::GetProjectDirectory() const
{
    return _projectDirectory;
}