#include "Application.h"

#include "ClassRegistrator.h"
#include "WindowContext.h"
#include <GLFW/glfw3.h>
#include <kotono_common/log.h>
#include <kotono_core/Interface.h>
#include <kotono_graphics/SpvCompiler.h>
#include <kotono_interface/Text.h>
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
    , mainWindowContext_{ nullptr }
    , secondaryWindowContexts_{}
    , averageUpdateTime_{}
    , now_{ SClock::Now() }
{
}

void UApplication::Run()
{
    Init();

    while (!mainWindowContext_->GetShouldClose())
    {
        std::erase_if(secondaryWindowContexts_, [this](USecondaryWindowContext* windowContext) {
            if (windowContext->GetShouldClose())
            {
                vkDeviceWaitIdle(device_.GetDevice());
                windowContext->Cleanup();
                delete windowContext;
                return true;
            }
            return false;
        });

        glfwPollEvents();

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

    mainWindowContext_ = new UMainWindowContext{ context_, device_ };

    context_.Init();
    mainWindowContext_->InitSurface();
    device_.Init(mainWindowContext_->GetSurface());
    mainWindowContext_->InitRenderer();

    mainWindowContext_->InitInput();
    mainWindowContext_->InitInterface();

    secondaryWindowContexts_.push_back(new USecondaryWindowContext{ context_, device_ });
    secondaryWindowContexts_.back()->Init({ 640u, 480u }, UCreate<WText>{}("Secondary Window!"));

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
    for (auto* windowContext : secondaryWindowContexts_)
    {
        windowContext->Cleanup();
        delete windowContext;
    }
    secondaryWindowContexts_.clear();

    mainWindowContext_->Cleanup();
    delete mainWindowContext_;

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
    f32 const deltaTime{ now - now_ };
    now_ = now;

    averageUpdateTime_.Add(deltaTime);
    logUPSTimer_.Update(deltaTime);

    std::vector<UInterface::PendingWindow> pendingWindows{};

    auto const updateAndPopulatePendingWindows{ [&pendingWindows, deltaTime](UWindowContext* windowContext) {
        windowContext->Update(deltaTime);
        pendingWindows.append_range(windowContext->GetInterface()->GetPendingWindows());
        windowContext->GetInterface()->ClearPendingWindows();
    } };

    updateAndPopulatePendingWindows(mainWindowContext_);
    std::ranges::for_each(secondaryWindowContexts_, updateAndPopulatePendingWindows);

    for (auto const& [widget, windowExtent] : pendingWindows)
    {
        secondaryWindowContexts_.push_back(new USecondaryWindowContext{ context_, device_ });
        secondaryWindowContexts_.back()->Init(windowExtent, widget);
    }
}

void UApplication::DrawFrame()
{
    mainWindowContext_->DrawFrame();

    for (auto* windowContext : secondaryWindowContexts_)
    {
        windowContext->DrawFrame();
    }
}

void UApplication::LogUPS() const
{
    KT_LOG(ELogImportanceLevel::High, "Application", "{0:.2f} ups", 1.0f / averageUpdateTime_.Get());
}
