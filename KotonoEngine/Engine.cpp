#include "Engine.h"
#include "Time.h"
#include "ObjectManager.h"

KEngine Engine;

static KTime Time;
static KObjectManager ObjectManager;

void KEngine::Init()
{
    Time.Init();
    ObjectManager.Init();
}

void KEngine::Cleanup()
{
    Time.Cleanup();
    ObjectManager.Cleanup();
}

KTime& KEngine::GetTime()
{
    return Time;
}

KObjectManager& KEngine::GetObjectManager()
{
    return ObjectManager;
}
