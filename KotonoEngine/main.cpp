#include "KotonoEngine.h"

KotonoEngine* Engine = new KotonoEngine();

int main()
{
    Engine->Run();

    delete Engine;

    return 0;
}