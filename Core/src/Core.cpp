#include "Core.h"
#include "Game.h"
#include "InterfacePhysicsManager.h"
#include "TimeManager.h"
#include <kotono_audio/AudioManager.h>
#include <kotono_common/AssetManager.h>
#include <kotono_graphics/TextureManager.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#include <kotono_object/Object.h>
#include <kotono_object/ObjectManager.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/Window.h>
#include <kotono_timing/TimerManager.h>
#include <kotono_graphics/Model.h>

void SCore::Init()
{
    Window.Init();
    Context.Init();
    Renderer.Init();
    ShaderManager.Init();
    AudioManager.Init();
    Keyboard.Init();
    Mouse.Init();
    TimeManager.Init();
    ObjectManager.Init();
    Game.Init();

    auto& logUPSTimer{ TimerManager.GetTimer("log ups timer") };
    logUPSTimer.SetDuration(1.0f);
    logUPSTimer.SetIsRepeat(true);
    logUPSTimer.EventCompleted().AddListener(this, &SCore::LogUPS);
    logUPSTimer.Start();
}

void SCore::Update()
{
    Keyboard.Update();
    Mouse.Update();
    TimeManager.Update();
    InterfacePhysicsManager.Update();
}

void SCore::Cleanup()
{
    Game.Cleanup();
#if defined(_DEBUG)
    KObject::CheckDebugRegistry();
#endif

    ObjectManager.Cleanup(); // todo: remove that prob
    AudioManager.Cleanup();
    TextureManager.Cleanup();
    ShaderManager.Cleanup();
    //ModelManager.Cleanup();
    UAssetManager<KtModel>::Cleanup();
    Renderer.Cleanup();
    Context.Cleanup();
    Window.Cleanup();
}

void SCore::LogUPS() const
{
    KT_LOG(ELogImportanceLevel::High, "Core", "{:.2f} ups", 1.0f / TimeManager.AverageUpdateTime());
}
