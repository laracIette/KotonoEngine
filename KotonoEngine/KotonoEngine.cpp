#include "KotonoEngine.h"

KotonoEngine::KotonoEngine() :
    _window(nullptr),
    _audioManager(nullptr)
{
}

KotonoEngine::~KotonoEngine()
{
    // Cleanup
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

    // MainLoop
    _window->MainLoop();
}

Window* KotonoEngine::GetWindow() const
{
    return _window;
}

AudioManager* KotonoEngine::GetAudioManager() const
{
    return _audioManager;
}
