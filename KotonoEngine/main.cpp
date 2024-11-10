#include "AudioManager.h"
#include "Window.h"
#include "Transform.h"
#include <iostream>

int main()
{
    // Init
    auto window = new Window();
    window->Init();

    auto audioManager = new AudioManager();
    audioManager->Init();

    // MainLoop
    window->MainLoop();

    // Cleanup
    delete audioManager;
    delete window;

    return 0;
}
