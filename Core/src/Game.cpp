#include "Game.h"
#include "Interface.h"
#include "ProjectSettings.h"
#include "Scene.h"
#include "TimeManager.h"
#include <kotono_common/log.h>
#include <kotono_input/Keyboard.h>
#include <kotono_object/ObjectFactory.h>

void GGame::Init()
{
	Keyboard.EventKey(EKey::S, EInputState::Pressed).AddListener(this, &GGame::OnKeySPressed);

    //OpenStartupInterface();
    OpenStartupScene();
}

void GGame::Update(const float deltaTime)
{
    if (interface_)
    {
        interface_->Update(deltaTime);
    }
    if (scene_)
    {
        scene_->Update(deltaTime);
    }
}

void GGame::Cleanup()
{
    if (interface_)
    {
        interface_->Delete();
    }
    if (scene_)
    {
        scene_->Delete();
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

void GGame::OpenInterface(const UPtr<KInterface>& interface)
{
    interface_ = interface;
}

void GGame::OpenScene(const UPtr<KScene>& scene)
{
    scene_ = scene;
}

void GGame::OpenStartupInterface()
{
    interface_ = UCreate<KInterface>{}();
    interface_->SpawnInterfaceObjects();
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

EGameState GGame::GetState() const
{
    return state_;
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

UEvent<EGameState>& GGame::EventStateChanged()
{
    return eventStateChanged_;
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
