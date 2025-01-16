#include "KotonoEngine.h"

KotonoEngine::KotonoEngine() :
    _window(nullptr),
    _audioManager(nullptr),
    _inputManager(nullptr),
    _projectDirectory(R"(C:\Users\nicos\Documents\Kotono Projects\Test)")
{
}

KotonoEngine::~KotonoEngine()
{
    delete _inputManager;
    delete _audioManager;
    delete _window;
}

void KotonoEngine::Run()
{
    // Init
    _window = new Window();
    _window->Init();

    _audioManager = new AudioManager();
    _audioManager->Init();

    _inputManager = new InputManager();
}

Window* KotonoEngine::GetWindow() const
{
    return _window;
}

AudioManager* KotonoEngine::GetAudioManager() const
{
    return _audioManager;
}

InputManager* KotonoEngine::GetInputManager() const
{
    return _inputManager;
}

const std::string& KotonoEngine::GetProjectDirectory() const
{
    return _projectDirectory;
}