#include "Core.h"
#include "DirectionalLightComponent.h"
#include "Game.h"
#include "InterfacePhysicsManager.h"
#include "PointLightComponent.h"
#include "TimeManager.h"
#include <kotono_audio/AudioManager.h>
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/Sampler.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/SpvCompiler.h>
#include <kotono_graphics/Texture.h>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/Window.h>
#include <kotono_timing/Timer.h>
#include <kotono_timing/TimerManager.h>

void GCore::Init()
{
    SSpvCompiler::CompileUpdated();

    Window.Init();
    Context.Init();
    Renderer.Init();
    AudioManager.Init();
    Keyboard.Init();
    Mouse.Init();
    TimeManager.Init();
    Game.Init();

    auto& logUPSTimer{ TimerManager.GetTimer("log ups timer") };
    logUPSTimer.SetDuration(1.0f);
    logUPSTimer.SetIsRepeat(true);
    logUPSTimer.EventCompleted().AddListener(this, &GCore::LogUPS);
    logUPSTimer.Start();

    // force unused classes to compile, for registry
    KDirectionalLightComponent{};
    KPointLightComponent{};
}

void GCore::Update()
{
    Keyboard.Update();
    Mouse.Update();
    TimeManager.Update();
    InterfacePhysicsManager.Update();
}

void GCore::Cleanup()
{
    Game.Cleanup();
    AudioManager.Cleanup();
    SAssetManager<USampler>::Cleanup();
    SAssetManager<UTexture>::Cleanup();
    SAssetManager<UShader>::Cleanup();
    SAssetManager<UModel>::Cleanup();
    Renderer.Cleanup();
    Context.Cleanup();
    Window.Cleanup();
}

void GCore::LogUPS() const
{
    KT_LOG(ELogImportanceLevel::High, "Core", "{:.2f} ups", 1.0f / TimeManager.AverageUpdateTime());
}
