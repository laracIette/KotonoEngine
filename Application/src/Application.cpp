#include "Application.h"
#include <kotono_audio/AudioManager.h>
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_core/DirectionalLightComponent.h>
#include <kotono_core/Game.h>
#include <kotono_core/PointLightComponent.h>
#include <kotono_core/TimeManager.h>
#include <kotono_platform/Window.h>
#ifdef EDITOR
    #include <kotono_editor/Editor.h>
#endif
#include <kotono_graphics/RenderContext.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/Sampler.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/SpvCompiler.h>
#include <kotono_graphics/Texture.h>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#ifndef NDEBUG
    #include <kotono_object/Object.h>
#endif
#include <kotono_platform/Context.h>
#include <kotono_platform/Window.h>
#include <kotono_timing/TimerManager.h>

void UApplication::Run() const
{
    Init();

    while (!Window.GetShouldClose())
    {
        Update();
    }

    Cleanup();
}

void UApplication::Init() const
{
    SSpvCompiler::CompileUpdated();

    Window.Init();
    Context.Init();
    RenderContext.Init();
    AudioManager.Init();
    Keyboard.Init();
    Mouse.Init();
    TimeManager.Init();

    auto& logUPSTimer{ TimerManager.GetTimer("log ups timer") };
    logUPSTimer.SetDuration(1.0f);
    logUPSTimer.SetIsRepeat(true);
    logUPSTimer.EventCompleted().AddListener(this, &UApplication::LogUPS);
    logUPSTimer.Start();

    // force unused classes to compile, for registry
    KDirectionalLightComponent{};
    KPointLightComponent{};

    Window.GetEventWindowResized().AddListener(this, &UApplication::OnWindowResized);

    RenderContext.GetViewport().SetKeepAspectRatio(false);
    RenderContext.GetViewport().SetExtent(Window.GetSize());

#   ifdef EDITOR
        Editor.Init();
#   endif

    Game.Init();
}

void UApplication::Update() const
{
    Keyboard.Update();
    Mouse.Update();
    TimeManager.Update();
}

void UApplication::Cleanup() const
{
    Game.Cleanup();

#   ifdef EDITOR
        Editor.Cleanup();
#   endif

    SAssetManager<USampler>::Cleanup();
    SAssetManager<UTexture>::Cleanup();
    SAssetManager<UShader>::Cleanup();
    SAssetManager<UModel>::Cleanup();

    AudioManager.Cleanup();
    RenderContext.Cleanup();
    Context.Cleanup();
    Window.Cleanup();

#   ifndef NDEBUG
        KObject::CheckDebugRegistry();
#   endif
}

void UApplication::LogUPS() const
{
    KT_LOG(ELogImportanceLevel::High, "Application", "{0:.2f} ups", 1.0f / TimeManager.AverageUpdateTime());
}

void UApplication::OnWindowResized(const glm::uvec2 extent) const
{
    RenderContext.GetViewport().SetExtent(extent);
}
