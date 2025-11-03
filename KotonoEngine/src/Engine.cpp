#include "Engine.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "Game.h"
#include "InterfacePhysicsManager.h"

static STimeManager TimeManager;
static SObjectManager ObjectManager;
static SGame Game;
static SInterfacePhysicsManager InterfacePhysicsManager;

void SEngine::Init()
{
    ::TimeManager.Init();
    ::ObjectManager.Init();
}

void SEngine::Update()
{
    ::TimeManager.Update();
    ::InterfacePhysicsManager.Update();
}

void SEngine::Cleanup()
{
    ::ObjectManager.Cleanup();
}

STimeManager& SEngine::TimeManager()
{
    return ::TimeManager;
}

const STimeManager& SEngine::TimeManager() const
{
    return ::TimeManager;
}

SObjectManager& SEngine::ObjectManager()
{
    return ::ObjectManager;
}

const SObjectManager& SEngine::ObjectManager() const
{
    return ::ObjectManager;
}

SGame& SEngine::Game() 
{
    return ::Game;
}

const SGame& SEngine::Game() const
{
    return ::Game;
}

SInterfacePhysicsManager& SEngine::InterfacePhysicsManager()
{
    return ::InterfacePhysicsManager;
}

const SInterfacePhysicsManager& SEngine::InterfacePhysicsManager() const
{
    return ::InterfacePhysicsManager;
}
