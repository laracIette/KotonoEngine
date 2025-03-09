#include "Engine.h"

void KEngine::Init()
{
    _time.Init();
    _objectManager.Init();
}

void KEngine::Update()
{
    _time.Update();
    _objectManager.Update();
}

void KEngine::Cleanup()
{
    _time.Cleanup();
    _objectManager.Cleanup();
}

KTime& KEngine::GetTime()
{
    return _time;
}

KObjectManager& KEngine::GetObjectManager()
{
    return _objectManager;
}
