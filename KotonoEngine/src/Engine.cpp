#include "Engine.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "Visualizer.h"
#include "Interface.h"
#include "InterfacePhysicsManager.h"

static STimeManager TimerManager;
static SObjectManager ObjectManager;
static SVisualizer Visualizer;
static SInterface Interface;
static SInterfacePhysicsManager InterfacePhysicsManager;

void SEngine::Init()
{
    TimerManager.Init();
    ObjectManager.Init();
    Visualizer.Init();
    Interface.Init();
}

void SEngine::Update()
{
    TimerManager.Update();
    ObjectManager.Update();
    InterfacePhysicsManager.Update();
}

void SEngine::Cleanup()
{
    Interface.Cleanup();
    TimerManager.Cleanup();
    ObjectManager.Cleanup();
    Visualizer.Cleanup();
}

STimeManager& SEngine::GetTimeManager()
{
    return TimerManager;
}

const STimeManager& SEngine::GetTimeManager() const
{
    return TimerManager;
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

SInterface& SEngine::GetInterface()
{
    return Interface;
}

const SInterface& SEngine::GetInterface() const
{
    return Interface;
}

SInterfacePhysicsManager& SEngine::GetInterfacePhysicsManager()
{
    return InterfacePhysicsManager;
}

const SInterfacePhysicsManager& SEngine::GetInterfacePhysicsManager() const
{
    return InterfacePhysicsManager;
}
