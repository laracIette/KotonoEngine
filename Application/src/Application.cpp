#include "Application.h"
#include <kotono_audio/AudioManager.h>
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_core/DirectionalLightComponent.h>
#include <kotono_core/Game.h>
#include <kotono_core/PointLightComponent.h>
#include <kotono_core/TimeManager.h>
#include <kotono_platform/Window.h>
#include <kotono_graphics/RenderContext.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/Sampler.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/SpvCompiler.h>
#include <kotono_graphics/Texture.h>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#include <kotono_interface/Interface.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/Window.h>
#include <kotono_timing/TimerManager.h>
#ifdef EDITOR
    #include <kotono_editor/Camera.h>
    #include <kotono_editor/Visualizer.h>
    #include <kotono_editor/MainWindow.h>
#endif
#ifndef NDEBUG
    #include <kotono_object/Object.h>
#endif

void UApplication::Run()
{
    Init();

    while (!Window.GetShouldClose())
    {
        Update();
    }

    Cleanup();
}

void UApplication::Init()
{
    SSpvCompiler::CompileUpdated();

    Window.Init();
    Context.Init();
    RenderContext.Init();
    AudioManager.Init();
    Keyboard.Init();
    Mouse.Init();
    TimeManager.Init();
    Interface.Init();

    RenderContext.GetViewport().SetKeepAspectRatio(false);
    RenderContext.GetViewport().SetExtent(Window.GetSize());

    auto& logUPSTimer{ TimerManager.GetTimer("log ups timer") };
    logUPSTimer.SetDuration(1.0f);
    logUPSTimer.SetIsRepeat(true);
    logUPSTimer.EventCompleted().AddListener(this, &UApplication::LogUPS);
    logUPSTimer.Start();

    Window.GetEventWindowResized().AddListener(this, &UApplication::OnWindowResized);

#   ifdef EDITOR
        Visualizer.Init();
        Camera.Init();

        auto& updateTimer{ TimerManager.GetTimer("update time text") };
        updateTimer.SetDuration(1.0f / 20.0f);
        updateTimer.SetIsRepeat(true);
        updateTimer.Start();

        mainWindow_ = UCreate<WMainWindow>{ "Main Window" }();
        mainWindow_->BeginDraw();
#   endif

    Game.Init();

    // force unused classes to compile, for registry
    {
        KDirectionalLightComponent{};
        KPointLightComponent{};
    }
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
        Camera.Cleanup();

        if (mainWindow_)
        {
            mainWindow_->EndDraw();
            mainWindow_->Delete();
        }
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
