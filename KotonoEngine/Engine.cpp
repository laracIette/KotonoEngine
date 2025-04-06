#include "Engine.h"

KEngine Engine;

void KEngine::Init()
{
    _time.Init();
    _objectManager.Init();
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
