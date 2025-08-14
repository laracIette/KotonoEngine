#include "Application.h"
#include <kotono_framework/Framework.h>
#include "Engine.h"

constinit static bool IsRunning{ false };

void SApplication::Run()
{
    if (IsRunning)
    {
        return;
    }

    IsRunning = true;

    SApplication application{};
    application.Init();
    application.MainLoop();
    application.Cleanup();

    IsRunning = false;
}

void SApplication::Init()
{
    Framework.Init();
    Engine.Init();
}

void SApplication::MainLoop()
{
    while (!Framework.GetWindow().GetShouldClose())
    {
        Framework.Update();
        Engine.Update();
    }
}

void SApplication::Cleanup()
{
    Engine.Cleanup();
    Framework.Cleanup();
}
