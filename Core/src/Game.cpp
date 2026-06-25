#include "Game.h"
#include "Interface.h"
#include "ProjectSettings.h"
#include "Scene.h"
#include "TimeManager.h"
#include <kotono_common/log.h>
#include <kotono_input/Keyboard.h>
#include <kotono_object/ObjectFactory.h>

void SGame::Init()
{
	Keyboard.EventKey(EKey::S, EInputState::Pressed).AddListener(this, &SGame::OnKeySPressed);

    //OpenStartupInterface();
    OpenStartupScene();
}

void SGame::Update(const float deltaTime)
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

void SGame::Cleanup()
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

bool SGame::IsPlaying() const
{
    return state_ == EGameState::Playing;
}

bool SGame::IsPaused() const
{
    return state_ == EGameState::Paused;
}

bool SGame::IsStopped() const
{
    return state_ == EGameState::Stopped;
}

void SGame::Play()
{
    SetState(EGameState::Playing);
}

void SGame::Pause()
{
    SetState(EGameState::Paused);
}

void SGame::Stop()
{
    SetState(EGameState::Stopped);
}

void SGame::OpenInterface(const UPtr<KInterface>& interface)
{
    interface_ = interface;
}

void SGame::OpenScene(const UPtr<KScene>& scene)
{
    scene_ = scene;
}

void SGame::OpenStartupInterface()
{
    interface_ = UCreate<KInterface>{}();
    interface_->SpawnInterfaceObjects();
}

void SGame::OpenStartupScene()
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

EGameState SGame::GetState() const
{
    return state_;
}

const UPtr<KScene>& SGame::GetOpenedScene() const
{
    return scene_;
}

void SGame::SetState(const EGameState state)
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

UEvent<EGameState>& SGame::EventStateChanged()
{
    return eventStateChanged_;
}

void SGame::OnKeySPressed() const
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
