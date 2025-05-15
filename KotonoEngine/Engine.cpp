#include "Engine.h"
#include "Time.h"
#include "ObjectManager.h"
#include "Visualizer.h"

SEngine Engine;

static STime Time;
static SObjectManager ObjectManager;
static SVisualizer Visualizer;

void SEngine::Init()
{
    Time.Init();
    ObjectManager.Init();
    Visualizer.Init();
}

void SEngine::Cleanup()
{
    Time.Cleanup();
    ObjectManager.Cleanup();
    Visualizer.Cleanup();
}

STime& SEngine::GetTime()
{
    return Time;
}

SObjectManager& SEngine::GetObjectManager()
{
    return ObjectManager;
}

SVisualizer& SEngine::GetVisualizer()
{
    return Visualizer;
}
