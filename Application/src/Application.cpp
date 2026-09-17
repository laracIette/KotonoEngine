#include "Application.h"

#include "ClassRegistrator.h"
#include <GLFW/glfw3.h>
#include <kotono_common/log.h>
#include <kotono_graphics/SpvCompiler.h>
#include <kotono_timing/Clock.h>

#ifdef EDITOR
#include <kotono_editor/Visualizer.h>
#endif

#ifndef NDEBUG
#include <kotono_core/Object.h>
#endif

UApplication::UApplication()
    : context_{}
    , device_{ context_ }
    , mainWindowContext_{ context_, device_ }
{
}

void UApplication::Run()
{
    Init();

    while (!mainWindowContext_.GetShouldClose())
    {
        Update();
        DrawFrame();
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

    RegisterObjectClasses();

    context_.Init();
    mainWindowContext_.InitSurface();
    device_.Init(mainWindowContext_.GetSurface());
    mainWindowContext_.InitRenderer();

    mainWindowContext_.InitInput();
    mainWindowContext_.InitInterface();

    logUPSTimer_.SetDuration(1.0f);
    logUPSTimer_.SetIsRepeat(true);
    logUPSTimer_.EventCompleted().AddListener(this, &UApplication::LogUPS);
    logUPSTimer_.Start();

#   ifdef EDITOR
    Visualizer.Init();
#   endif
}

void UApplication::Cleanup()
{
    mainWindowContext_.Cleanup();

    device_.Cleanup();
    context_.Cleanup();

    glfwTerminate();

#   ifndef NDEBUG
    KObject::CheckDebugRegistry();
#   endif
}

void UApplication::Update()
{
    f32 const now{ SClock::Now() };
    deltaTime_ = now - now_;
    now_ = now;
    averageUpdateTime_.Add(deltaTime_);

    logUPSTimer_.Update(deltaTime_);

    mainWindowContext_.Update(deltaTime_);
}

void UApplication::DrawFrame()
{
    //for (auto const& [widget, windowExtent] : interface_->GetPendingWindows())
    //{
    //    UWindow window{};
    //    USurface surface{ window, context_ };
    //    URenderer renderer{ device_, surface };
    //}
    //interface_->ClearPendingWindows();

    mainWindowContext_.DrawFrame();
}

void UApplication::LogUPS() const
{
    KT_LOG(ELogImportanceLevel::High, "Application", "{0:.2f} ups", 1.0f / averageUpdateTime_.Get());
}
