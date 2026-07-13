#include "Game.h"
#include "MeshComponent.h"
#include "ProjectSettings.h"
#include "Scene.h"
#include "SceneObject.h"
#include "TimeManager.h"
#include <kotono_common/Asset.h>
#include <kotono_common/AssetManager.h>
#include <kotono_common/log.h>
#include <kotono_graphics/Material.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/Shader.h>
#include <kotono_input/Keyboard.h>
#include <kotono_object/ObjectFactory.h>
#include <kotono_platform/glm_utils.h>

static UPtr<TSceneObject> TestSceneObject;

void GGame::Init()
{
	Keyboard.EventKey(EKey::S, EInputState::Pressed).AddListener(this, &GGame::OnKeySPressed);

    OpenStartupScene();
    OpenTestScene();
}

void GGame::Update(const float deltaTime)
{
    if (scene_)
    {
        scene_->Update(deltaTime);
    }
}

void GGame::Cleanup()
{
    if (scene_)
    {
        scene_->Delete();
    }
    if (TestSceneObject)
    {
        TestSceneObject->Delete();
    }
}

bool GGame::IsPlaying() const
{
    return state_ == EGameState::Playing;
}

bool GGame::IsPaused() const
{
    return state_ == EGameState::Paused;
}

bool GGame::IsStopped() const
{
    return state_ == EGameState::Stopped;
}

void GGame::Play()
{
    SetState(EGameState::Playing);
}

void GGame::Pause()
{
    SetState(EGameState::Paused);
}

void GGame::Stop()
{
    SetState(EGameState::Stopped);
}

EGameState GGame::GetState() const
{
    return state_;
}

UEvent<EGameState>& GGame::GetEventStateChanged()
{
    return eventStateChanged_;
}

const UPtr<KScene>& GGame::GetOpenedScene() const
{
    return scene_;
}

void GGame::SetState(const EGameState state)
{
    if (state_ == state)
    {
        return;
    }

    state_ = state;

    switch (state_)
    {
    case EGameState::Playing:
    {
        TimeManager.GameTime().state = ETimeContextState::Playing;
        if (UPtr scene{ GetOpenedScene() })
        {
            scene->Serialize();
        }
        break;
    }
    case EGameState::Stopped:
        TimeManager.GameTime().total = 0.0f;
        TimeManager.GameTime().currentDelta = 0.0f;
        OpenStartupScene();
        [[fallthrough]];
    case EGameState::Paused:
        TimeManager.GameTime().state = ETimeContextState::Paused;
        break;
    }

    eventStateChanged_.Broadcast(state_);
}

void GGame::OpenScene(const UPtr<KScene>& scene)
{
    scene_ = scene;
}

void GGame::OpenStartupScene()
{
    if (scene_)
    {
        scene_->Delete();
    }
    const auto startupScene{ SProjectSettings::Get<std::string>("/startupScene") };
    if (scene_ = TryCast<KScene>(SObjectFactory::Get().Get(startupScene)))
    {
        scene_->SpawnSceneObjects();
    }
}

void GGame::OpenTestScene() const
{
    bool isModel1{ true };
    UAsset model1{ SAssetManager<UModel>::Get("${ENGINE_DIRECTORY}/Graphics/assets/models/viking_room.obj") };
    UAsset model2{ SAssetManager<UModel>::Get("${ENGINE_DIRECTORY}/Graphics/assets/models/column.obj") };
    UAsset shader{ SAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/gbuffer.kasset") };
    UAsset material1{ SAssetManager<UMaterial>::Get("${ENGINE_DIRECTORY}/Graphics/assets/materials/viking_room.kasset") };
    UAsset material2{ SAssetManager<UMaterial>::Get("${ENGINE_DIRECTORY}/Graphics/assets/materials/uv_grid.kasset") };

    TestSceneObject = UCreate<TSceneObject>{}();
    TestSceneObject->AddComponent(UCreate<KSceneComponent>{}());

    for (size i{ 100 }; i < 600; i++)
    {
        UPtr meshComponent{ UCreate<KMeshComponent>{}() };

        meshComponent->SetModel(isModel1 ? model1 : model2);
        meshComponent->SetShader(shader);
        meshComponent->SetMaterial(isModel1 ? material1 : material2);

        const glm::vec3 position{ std::cos(i) * i * 0.1f, i * 0.01f - 2.0f, std::sin(i) * i * 0.1f };
        meshComponent->SetWorldPosition(position);
        meshComponent->SetWorldRotation(glm::angleAxis(std::cos((float)i), WorldUpVector));

        TestSceneObject->AddComponent(meshComponent);

        isModel1 = !isModel1;
    }

    TestSceneObject->Spawn();
}

void GGame::OnKeySPressed() const
{
    if (!Keyboard.KeyState(EKey::LeftControl, EInputState::Down))
    {
        return;
    }

    if (scene_)
    {
        scene_->Serialize();
        KT_LOG(ELogImportanceLevel::High, "Core", "scene serialized");
    }
}
