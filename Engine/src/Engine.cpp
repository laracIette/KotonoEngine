#include "Engine.h"
#include <kotono_common/Path.h>
#include <kotono_audio/AudioManager.h>
#include <kotono_timing/TimeManager.h>
#include <kotono_platform/Window.h>
#include <kotono_platform/Context.h>
#include <kotono_input/InputManager.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/ModelManager.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/ImageTextureManager.h>
#include "TimeManager.h"
#include "ObjectManager.h"
#include "ObjectFactory.h"
#include "Game.h"
#include "InterfacePhysicsManager.h"

static STimeManager sTimeManager;
static SObjectManager sObjectManager;
static SObjectFactory sObjectFactory;
static SGame sGame;
static SInterfacePhysicsManager sInterfacePhysicsManager;

void SEngine::Init()
{
    ::Path.Init();
    ::Window.Init();
    ::Context.Init();
    ::Renderer.Init();
    ::ShaderManager.Init();
    ::AudioManager.Init();
    ::InputManager.Init();
    sTimeManager.Init();
    sObjectManager.Init();
    sGame.Init();
}

void SEngine::Update()
{
    ::TimeManager.Update();
    ::InputManager.Update();
    sTimeManager.Update();
    sGame.Update();
    sInterfacePhysicsManager.Update();
}

void SEngine::Cleanup()
{
    sObjectManager.Cleanup();
    ::AudioManager.Cleanup();
    ::ImageTextureManager.Cleanup();
    ::ShaderManager.Cleanup();
    ::ModelManager.Cleanup();
    ::Renderer.Cleanup();
    ::Context.Cleanup();
    ::Window.Cleanup();
}

STimeManager& SEngine::TimeManager()
{
    return sTimeManager;
}

const STimeManager& SEngine::TimeManager() const
{
    return sTimeManager;
}

SObjectManager& SEngine::ObjectManager()
{
    return sObjectManager;
}

const SObjectManager& SEngine::ObjectManager() const
{
    return sObjectManager;
}

SObjectFactory& SEngine::ObjectFactory()
{
    return sObjectFactory;
}

const SObjectFactory& SEngine::ObjectFactory() const
{
    return sObjectFactory;
}

SGame& SEngine::Game()
{
    return sGame;
}

const SGame& SEngine::Game() const
{
    return sGame;
}

SInterfacePhysicsManager& SEngine::InterfacePhysicsManager()
{
    return sInterfacePhysicsManager;
}

const SInterfacePhysicsManager& SEngine::InterfacePhysicsManager() const
{
    return sInterfacePhysicsManager;
}
