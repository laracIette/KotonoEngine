#include "Application.h"

#include "ClassRegistrator.h"
#include <GLFW/glfw3.h>
#include <kotono_common/log.h>
#include <kotono_core/Interface.h>
#include <kotono_graphics/InterfaceRenderGraph.h>
#include <kotono_graphics/SceneRenderGraph.h>
#include <kotono_graphics/SpvCompiler.h>
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
    , mouse_{ window_ }
    , keyboard_{ window_ }
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

    mouse_.Init();
    keyboard_.Init();

    RegisterObjectClasses();

    logUPSTimer_.SetDuration(1.0f);
    logUPSTimer_.SetIsRepeat(true);
    logUPSTimer_.EventCompleted().AddListener(this, &UApplication::LogUPS);
    logUPSTimer_.Start();

    window_.GetEventWindowResized().AddListener(this, &UApplication::OnWindowResized);

    interface_ = new UInterface{};

    mouse_.GetEventButton().AddListener(interface_, &UInterface::OnMouseButton);
    mouse_.GetEventMove().AddListener(interface_, &UInterface::OnMouseMove);
    mouse_.GetEventScroll().AddListener(interface_, &UInterface::OnMouseScroll);

    keyboard_.GetEventKey().AddListener(interface_, &UInterface::OnKeyboardKey);

#   ifdef EDITOR
    Visualizer.Init();
    interface_->SetWidget(UCreate<WMainWindow>{ "Main Window" }(interface_));
#   endif

    interface_->BeginDraw(window_.GetSize());
}

void UApplication::Update()
{
    // Game
    {
        f32 const now{ SClock::Now() };
        deltaTime_ = now - now_;
        now_ = now;
        averageUpdateTime_.Add(deltaTime_);

        mouse_.Update();
        keyboard_.Update();

        logUPSTimer_.Update(deltaTime_);
        interface_->Update(deltaTime_, mouse_.GetCursorPosition());
    }

    // Rendering
    {
        //for (auto const& [widget, windowExtent] : interface_->GetPendingWindows())
        //{
        //    UWindow window{};
        //    USurface surface{ window, context_ };
        //    URenderer renderer{ device_, surface };
        //}
        //interface_->ClearPendingWindows();

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
        mouse_.GetEventButton().RemoveListener(interface_, &UInterface::OnMouseButton);
        mouse_.GetEventMove().RemoveListener(interface_, &UInterface::OnMouseMove);
        mouse_.GetEventScroll().RemoveListener(interface_, &UInterface::OnMouseScroll);

        keyboard_.GetEventKey().RemoveListener(interface_, &UInterface::OnKeyboardKey);

        interface_->EndDraw();
        delete interface_;
    }

    mouse_.Cleanup();
    keyboard_.Cleanup();

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
