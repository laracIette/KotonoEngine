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

const STime& SEngine::GetTime() const
{
    return Time;
}

SObjectManager& SEngine::GetObjectManager()
{
    return ObjectManager;
}

const SObjectManager& SEngine::GetObjectManager() const
{
    return ObjectManager;
}

SVisualizer& SEngine::GetVisualizer()
{
    return Visualizer;
}

const SVisualizer& SEngine::GetVisualizer() const
{
    return Visualizer;
}
