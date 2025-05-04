#include "Engine.h"
#include "Time.h"
#include "ObjectManager.h"
#include "Visualizer.h"

KEngine Engine;

static KTime Time;
static KObjectManager ObjectManager;
static KVisualizer Visualizer;

void KEngine::Init()
{
    Time.Init();
    ObjectManager.Init();
    Visualizer.Init();
}

void KEngine::Cleanup()
{
    Time.Cleanup();
    ObjectManager.Cleanup();
    Visualizer.Cleanup();
}

KTime& KEngine::GetTime()
{
    return Time;
}

KObjectManager& KEngine::GetObjectManager()
{
    return ObjectManager;
}

KVisualizer& KEngine::GetVisualizer()
{
    return Visualizer;
}
