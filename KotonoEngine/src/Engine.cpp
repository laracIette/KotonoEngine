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
    ::TimerManager.Init();
    ::ObjectManager.Init();
    ::Visualizer.Init();
    ::Interface.Init();
}

void SEngine::Update()
{
    ::TimerManager.Update();
    ::ObjectManager.Update();
    ::InterfacePhysicsManager.Update();
}

void SEngine::Cleanup()
{
    ::Interface.Cleanup();
    ::TimerManager.Cleanup();
    ::ObjectManager.Cleanup();
    ::Visualizer.Cleanup();
}

STimeManager& SEngine::TimeManager()
{
    return ::TimerManager;
}

const STimeManager& SEngine::TimeManager() const
{
    return ::TimerManager;
}

SObjectManager& SEngine::ObjectManager()
{
    return ::ObjectManager;
}

const SObjectManager& SEngine::ObjectManager() const
{
    return ::ObjectManager;
}

SVisualizer& SEngine::Visualizer()
{
    return ::Visualizer;
}

const SVisualizer& SEngine::Visualizer() const
{
    return ::Visualizer;
}

SInterface& SEngine::Interface()
{
    return ::Interface;
}

const SInterface& SEngine::Interface() const
{
    return ::Interface;
}

SInterfacePhysicsManager& SEngine::InterfacePhysicsManager()
{
    return ::InterfacePhysicsManager;
}

const SInterfacePhysicsManager& SEngine::InterfacePhysicsManager() const
{
    return ::InterfacePhysicsManager;
}
