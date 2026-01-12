#include "GameManager.h"
#include <kotono_core/Game.h>
#include <kotono_core/TimeManager.h>
#include <kotono_core/Scene.h>
#include <kotono_timing/TimeContext.h>

bool SGameManager::IsPlaying() const
{
    return state_ == EGameState::Playing;
}

bool SGameManager::IsPaused() const
{
    return state_ == EGameState::Paused;
}

bool SGameManager::IsStopped() const
{
    return state_ == EGameState::Stopped;
}

void SGameManager::Play()
{
    SetState(EGameState::Playing);
}

void SGameManager::Pause()
{
    SetState(EGameState::Paused);
}

void SGameManager::Stop()
{
    SetState(EGameState::Stopped);
}

EGameState SGameManager::GetState() const
{
    return state_;
}

void SGameManager::SetState(const EGameState state)
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
        if (UPtr scene{ Game.GetOpenedScene() })
        {
            scene->Serialize();
        }
        break;
    }
    case EGameState::Stopped:
        TimeManager.GameTime().total = 0.0f;
        TimeManager.GameTime().currentDelta = 0.0f;
        Game.OpenStartupScene();
        [[fallthrough]];
    case EGameState::Paused:
        TimeManager.GameTime().state = ETimeContextState::Paused;
        break;
    }

    eventStateChanged_.Broadcast(state_);
}

UEvent<EGameState>& SGameManager::EventStateChanged()
{
    return eventStateChanged_;
}
