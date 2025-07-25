#include "Application.h"
#include <kotono_framework/Framework.h>
#include "Engine.h"

KtFramework Framework;
SEngine Engine;

void SApplication::Run()
{
    if (isRunning_)
    {
        return;
    }

    isRunning_ = true;

    Init();
    MainLoop();
    Cleanup();

    isRunning_ = false;
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
