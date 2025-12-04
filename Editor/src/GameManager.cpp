#include "GameManager.h"
#include <kotono_engine/Engine.h>
#include <kotono_engine/TimeManager.h>
#include <kotono_framework/TimeContext.h>

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
        Engine.TimeManager().GameTime().state = KT_TIME_CONTEXT_STATE_PLAYING;
        break;
    case EGameState::Stopped:
        Engine.TimeManager().GameTime().total = 0.0f;
        [[fallthrough]];
    case EGameState::Paused:
        Engine.TimeManager().GameTime().state = KT_TIME_CONTEXT_STATE_PAUSED;
        break;
    }

    eventStateChanged_.Broadcast(state_);
}

KtEvent<EGameState>& SGameManager::EventStateChanged()
{
    return eventStateChanged_;
}
