#include "Application.h"

#include <kotono_audio/AudioManager.h>
#include <kotono_common/AssetManager.h>
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
#include <kotono_rendering/Material.h>
#include <kotono_rendering/Model.h>
#include <kotono_rendering/Sampler.h>
#include <kotono_rendering/Shader.h>
#include <kotono_rendering/Texture.h>
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

    // pre load resources
    {
        SAssetManager<ASampler>::Create("${ENGINE_DIRECTORY}/Graphics/assets/samplers/default.kasset");
        SAssetManager<ASampler>::Create("${ENGINE_DIRECTORY}/Graphics/assets/samplers/shadow.kasset");

        SAssetManager<ATexture>::Create("${ENGINE_DIRECTORY}/Graphics/assets/models/viking_room.png");
        SAssetManager<ATexture>::Create("${ENGINE_DIRECTORY}/Graphics/assets/textures/black_texture.jpg");
        SAssetManager<ATexture>::Create("${ENGINE_DIRECTORY}/Graphics/assets/textures/default_normal.jpg");
        SAssetManager<ATexture>::Create("${ENGINE_DIRECTORY}/Graphics/assets/textures/default_orm.jpg");
        SAssetManager<ATexture>::Create("${ENGINE_DIRECTORY}/Graphics/assets/textures/default_texture.jpg");
        SAssetManager<ATexture>::Create("${ENGINE_DIRECTORY}/Graphics/assets/textures/white_texture.jpg");
        
        SAssetManager<AMaterial>::Create("${ENGINE_DIRECTORY}/Graphics/assets/materials/ground.kasset");
        SAssetManager<AMaterial>::Create("${ENGINE_DIRECTORY}/Graphics/assets/materials/uv_grid.kasset");
        SAssetManager<AMaterial>::Create("${ENGINE_DIRECTORY}/Graphics/assets/materials/viking_room.kasset");
        
        SAssetManager<AModel>::Create("${ENGINE_DIRECTORY}/Graphics/assets/models/column.obj");
        SAssetManager<AModel>::Create("${ENGINE_DIRECTORY}/Graphics/assets/models/floor.obj");
        SAssetManager<AModel>::Create("${ENGINE_DIRECTORY}/Graphics/assets/models/rectangle.obj");
        SAssetManager<AModel>::Create("${ENGINE_DIRECTORY}/Graphics/assets/models/viking_room.obj");
        
        SAssetManager<AShader>::Create("${ENGINE_DIRECTORY}/Graphics/assets/shaders/clusterAABB.kasset", renderer_.GetSwapChainFormat());
        SAssetManager<AShader>::Create("${ENGINE_DIRECTORY}/Graphics/assets/shaders/deferredLighting.kasset", renderer_.GetSwapChainFormat());
        SAssetManager<AShader>::Create("${ENGINE_DIRECTORY}/Graphics/assets/shaders/depthPrePass.kasset", renderer_.GetSwapChainFormat());
        SAssetManager<AShader>::Create("${ENGINE_DIRECTORY}/Graphics/assets/shaders/flatColor2D.kasset", renderer_.GetSwapChainFormat());
        SAssetManager<AShader>::Create("${ENGINE_DIRECTORY}/Graphics/assets/shaders/gbuffer.kasset", renderer_.GetSwapChainFormat());
        SAssetManager<AShader>::Create("${ENGINE_DIRECTORY}/Graphics/assets/shaders/lightBinning.kasset", renderer_.GetSwapChainFormat());
        SAssetManager<AShader>::Create("${ENGINE_DIRECTORY}/Graphics/assets/shaders/postProcess.kasset", renderer_.GetSwapChainFormat());
        SAssetManager<AShader>::Create("${ENGINE_DIRECTORY}/Graphics/assets/shaders/shader2D.kasset", renderer_.GetSwapChainFormat());
        SAssetManager<AShader>::Create("${ENGINE_DIRECTORY}/Graphics/assets/shaders/shadowPrePass.kasset", renderer_.GetSwapChainFormat());
    }

    {
        constexpr std::array CHARACTER_NAMES
        {
            "a", "ua", "b", "ub",
            "c", "uc", "d", "ud",
            "e", "ue", "f", "uf",
            "g", "ug", "h", "uh",
            "i", "ui", "j", "uj",
            "k", "uk", "l", "ul",
            "m", "um", "n", "un",
            "o", "uo", "p", "up",
            "q", "uq", "r", "ur",
            "s", "us", "t", "ut",
            "u", "uu", "v", "uv",
            "w", "uw", "x", "ux",
            "y", "uy", "z", "uz",
            "0", "1", "2", "3", "4",
            "5", "6", "7", "8", "9",
            "hash", "comma", "dot", "colon", "apostrophe",
            "lbracket", "rbracket",
            "fslash", "bslash",
            "exclamation", "question",
            "plus", "minus",
            "space", "underscore",
            "lesser", "greater",
        };

        for (auto const& character : CHARACTER_NAMES)
        {
            SAssetManager<ATexture>::Create(std::format("${{ENGINE_DIRECTORY}}/Graphics/assets/fonts/default/{0}.png", character));
        }
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

    SAssetManager<ASampler>::Cleanup();
    SAssetManager<ATexture>::Cleanup();
    SAssetManager<AShader>::Cleanup();
    SAssetManager<AModel>::Cleanup();

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
