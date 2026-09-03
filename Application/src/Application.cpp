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
#include <kotono_object/Interface.h>
#include <kotono_object/ProjectSettings.h>
#include <kotono_timing/Clock.h>
#include <kotono_timing/TimerManager.h>

#ifdef EDITOR
#include <kotono_editor/MainWindow.h>
#include <kotono_editor/Visualizer.h>
#endif

#ifndef NDEBUG
#include <kotono_object/Object.h>
#endif

UApplication::UApplication()
    : window_{}
    , context_{}
    , surface_{ window_, context_ }
    , device_{ context_, surface_ }
    , renderer_{ device_, surface_ }
{
}

void UApplication::Run()
{
    Init();

    while (!window_.GetShouldClose(device_.GetDevice()))
    {
        Update();
    }

    Cleanup();
}

void UApplication::Init()
{
    SSpvCompiler::CompileUpdated();

    window_.Init();
    context_.Init();
    surface_.Init();
    device_.Init();
    renderer_.Init();

    AudioManager.Init();
    Keyboard.Init(window_);
    Mouse.Init(window_);

    auto& logUPSTimer{ TimerManager.GetTimer("log ups timer") };
    logUPSTimer.SetDuration(1.0f);
    logUPSTimer.SetIsRepeat(true);
    logUPSTimer.EventCompleted().AddListener(this, &UApplication::LogUPS);
    logUPSTimer.Start();

    window_.GetEventWindowResized().AddListener(this, &UApplication::OnWindowResized);

    interface_ = new UInterface{};

#   ifdef EDITOR
    Visualizer.Init();

    auto& updateTimer{ TimerManager.GetTimer("update time text") };
    updateTimer.SetDuration(1.0f / 20.0f);
    updateTimer.SetIsRepeat(true);
    updateTimer.Start();

    mainWindow_ = UCreate<WMainWindow>{ "Main Window" }(SProjectSettings::Get<std::string>("/startupScene"));
    mainWindow_->SetInterface(interface_);

    interface_->SetWidget(mainWindow_);

    Mouse.GetEventAnyButton().AddListener(mainWindow_.Get(), &WSceneContext::OnMouseButton);
    Mouse.GetEventMove().AddListener(mainWindow_.Get(), &WSceneContext::OnMouseMove);
#   endif

    interface_->BeginDraw(window_.GetSize());

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
        f32 const now{ SClock::Now() };
        deltaTime_ = now - now_;
        now_ = now;
        averageUpdateTime_.Add(deltaTime_);

        Keyboard.Update();
        Mouse.Update();

        //TimerManager.Update(deltaTime_);
        interface_->Update(deltaTime_);
    }

    // Rendering
    {
        UInterfaceRenderGraph interfaceRenderGraph{};
        interface_->PopulateInterfaceRenderGraph(interfaceRenderGraph);

        USceneRenderGraph sceneRenderGraph{};
        interface_->PopulateSceneRenderGraph(sceneRenderGraph);

        renderer_.DrawFrame(sceneRenderGraph, interfaceRenderGraph);
    }
}

void UApplication::Cleanup()
{
    if (interface_)
    {
        interface_->EndDraw();
        delete interface_;
    }

    AudioManager.Cleanup();

    renderer_.Cleanup();
    device_.Cleanup();
    surface_.Cleanup();
    context_.Cleanup();
    window_.Cleanup();

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
    if (interface_)
    {
        interface_->EndDraw();
        interface_->BeginDraw(extent);
    }
}
