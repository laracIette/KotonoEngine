#include "Core.h"
#include "Game.h"
#include "InterfacePhysicsManager.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include <kotono_audio/AudioManager.h>
#include <kotono_graphics/TextureManager.h>
#include <kotono_graphics/ModelManager.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/Window.h>

void SCore::Init()
{
    ::Window.Init();
    ::Context.Init();
    ::Renderer.Init();
    ::ShaderManager.Init();
    ::AudioManager.Init();
    ::Keyboard.Init();
    ::Mouse.Init();
    ::TimeManager.Init();
    ::ObjectManager.Init();
    ::Game.Init();
}

void SCore::Update()
{
    ::Keyboard.Update();
    ::Mouse.Update();
    ::TimeManager.Update();
    ::InterfacePhysicsManager.Update();
}

void SCore::Cleanup()
{
    ::ObjectManager.Cleanup();
    ::AudioManager.Cleanup();
    ::TextureManager.Cleanup();
    ::ShaderManager.Cleanup();
    ::ModelManager.Cleanup();
    ::Renderer.Cleanup();
    ::Context.Cleanup();
    ::Window.Cleanup();
}
