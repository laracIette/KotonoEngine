#include "Engine.h"
#include <kotono_common/Path.h>
#include <kotono_audio/AudioManager.h>
#include <kotono_timing/TimerManager.h>
#include <kotono_platform/Window.h>
#include <kotono_platform/Context.h>
#include <kotono_input/InputManager.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/ModelManager.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/ImageTextureManager.h>
#include "TimeManager.h"
#include "ObjectManager.h"
#include "Game.h"
#include "InterfacePhysicsManager.h"

void SEngine::Init()
{
    ::Path.Init();
    ::Window.Init();
    ::Context.Init();
    ::Renderer.Init();
    ::ShaderManager.Init();
    ::AudioManager.Init();
    ::InputManager.Init();
    ::TimeManager.Init();
    ::ObjectManager.Init();
    ::Game.Init();
}

void SEngine::Update()
{
    ::TimerManager.Update();
    ::InputManager.Update();
    ::TimeManager.Update();
    ::Game.Update();
    ::InterfacePhysicsManager.Update();
}

void SEngine::Cleanup()
{
    ::ObjectManager.Cleanup();
    ::AudioManager.Cleanup();
    ::ImageTextureManager.Cleanup();
    ::ShaderManager.Cleanup();
    ::ModelManager.Cleanup();
    ::Renderer.Cleanup();
    ::Context.Cleanup();
    ::Window.Cleanup();
}
