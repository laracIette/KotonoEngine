#include "Application.h"

#include <GLFW/glfw3.h>
#include <kotono_common/log.h>
#include <kotono_core/Interface.h>
#include <kotono_graphics/InterfaceRenderGraph.h>
#include <kotono_graphics/SceneRenderGraph.h>
#include <kotono_graphics/SpvCompiler.h>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#include <kotono_scene/DirectionalLightComponent.h>
#include <kotono_scene/MeshComponent.h>
#include <kotono_scene/PointLightComponent.h>
#include <kotono_timing/Clock.h>

#ifdef EDITOR
#include <kotono_editor/MainWindow.h>
#include <kotono_editor/Visualizer.h>
#endif

#ifndef NDEBUG
#include <kotono_core/Object.h>
#endif

UApplication::UApplication()
    : context_{}
    , device_{ context_ }
    , window_{}
    , surface_{ window_, context_ }
    , renderer_{ device_, surface_ }
{
}

void UApplication::Run()
{
    Init();

    while (!window_.GetShouldClose())
    {
        Update();
    }

    vkDeviceWaitIdle(device_.GetDevice());

    Cleanup();
}

void UApplication::Init()
{
    SSpvCompiler::CompileUpdated();

    if (glfwInit() == GLFW_FALSE)
    {
        throw std::runtime_error{ "Failed to initialize GLFW" };
    }

    context_.Init();
    window_.Init();
    surface_.Init();
    device_.Init(surface_.GetSurface());
    renderer_.Init();

    Keyboard.Init(window_);
    Mouse.Init(window_);

    logUPSTimer_.SetDuration(1.0f);
    logUPSTimer_.SetIsRepeat(true);
    logUPSTimer_.EventCompleted().AddListener(this, &UApplication::LogUPS);
    logUPSTimer_.Start();

    window_.GetEventWindowResized().AddListener(this, &UApplication::OnWindowResized);

    interface_ = new UInterface{};

#   ifdef EDITOR
    Visualizer.Init();

    mainWindow_ = UCreate<WMainWindow>{ "Main Window" }(interface_);

    interface_->SetWidget(mainWindow_);
#   endif

    interface_->BeginDraw(window_.GetSize());

    // force unused classes to compile, for registry
    {
        KDirectionalLightComponent{};
        KMeshComponent{};
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

        logUPSTimer_.Update(deltaTime_);
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

    renderer_.Cleanup();
    device_.Cleanup();
    surface_.Cleanup();
    context_.Cleanup();
    window_.Cleanup();

    glfwTerminate();

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
