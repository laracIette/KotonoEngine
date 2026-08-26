#include "Application.h"

#include <kotono_audio/AudioManager.h>
#include <kotono_common/log.h>
#include <kotono_core/DirectionalLightComponent.h>
#include <kotono_core/PointLightComponent.h>
#include <kotono_graphics/InterfaceRenderGraph.h>
#include <kotono_graphics/SceneRenderGraph.h>
#include <kotono_graphics/SpvCompiler.h>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#include <kotono_interface/Interface.h>
#include <kotono_object/Interface.h>
#include <kotono_object/ProjectSettings.h>
#include <kotono_platform/Context.h>
#include <kotono_platform/Window.h>
#include <kotono_timing/Clock.h>
#include <kotono_timing/TimerManager.h>

#ifdef EDITOR
#include <kotono_editor/MainWindow.h>
#include <kotono_editor/Visualizer.h>
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
    renderer_.Init();

    AudioManager.Init();
    Keyboard.Init();
    Mouse.Init();
    Interface.Init();

    auto& logUPSTimer{ TimerManager.GetTimer("log ups timer") };
    logUPSTimer.SetDuration(1.0f);
    logUPSTimer.SetIsRepeat(true);
    logUPSTimer.EventCompleted().AddListener(this, &UApplication::LogUPS);
    logUPSTimer.Start();

    Window.GetEventWindowResized().AddListener(this, &UApplication::OnWindowResized);

    interface_ = new UInterface{};

#   ifdef EDITOR
    Visualizer.Init();

    auto& updateTimer{ TimerManager.GetTimer("update time text") };
    updateTimer.SetDuration(1.0f / 20.0f);
    updateTimer.SetIsRepeat(true);
    updateTimer.Start();

    mainWindow_ = UCreate<WMainWindow>{ "Main Window" }(SProjectSettings::Get<std::string>("/startupScene"));
    mainWindow_->SetInterface(interface_);
    mainWindow_->BeginDraw(Window.GetSize());
#   endif

    // force unused classes to compile, for registry
    {
        KDirectionalLightComponent{};
        KPointLightComponent{};
    }
}

void UApplication::Update()
{
    // Game
    {
        Keyboard.Update();
        Mouse.Update();

        f32 const now{ SClock::Now() };
        deltaTime_ = now - now_;
        now_ = now;
        averageUpdateTime_.Add(deltaTime_);

        interface_->Update(deltaTime_);
        mainWindow_->Update(deltaTime_);
    }

    // Rendering
    {
        UInterfaceRenderGraph interfaceRenderGraph{};
        mainWindow_->PopulateRenderGraph(interfaceRenderGraph);

        USceneRenderGraph sceneRenderGraph{};
        mainWindow_->PopulateSceneRenderGraph(sceneRenderGraph);

        renderer_.RegisterPendingTextures(interface_->GetPendingTextures());
        interface_->ClearPendingTextures();

        renderer_.RegisterPendingSceneRenders(interface_->GetPendingSceneRenders());
        interface_->ClearPendingSceneRenders();

        renderer_.UnregisterUnusedSceneRenders(interface_->GetUnusedSceneRenders());
        interface_->ClearUnusedSceneRenders();

        renderer_.DrawFrame(interface_->GetSceneViews(), sceneRenderGraph, interfaceRenderGraph);
    }
}

void UApplication::Cleanup()
{
#   ifdef EDITOR
    if (mainWindow_)
    {
        mainWindow_->EndDraw();
        mainWindow_->Delete();
    }
#   endif

    AudioManager.Cleanup();

    renderer_.Cleanup();
    Context.Cleanup();
    Window.Cleanup();

#   ifndef NDEBUG
    KObject::CheckDebugRegistry();
#   endif
}

void UApplication::LogUPS() const
{
    KT_LOG(ELogImportanceLevel::High, "Application", "{0:.2f} ups", 1.0f / averageUpdateTime_.Get());
}

void UApplication::OnWindowResized(glm::uvec2 const& extent)
{
#   ifdef EDITOR
    if (mainWindow_)
    {
        mainWindow_->EndDraw();
        mainWindow_->BeginDraw(extent);
    }
#   endif
}
