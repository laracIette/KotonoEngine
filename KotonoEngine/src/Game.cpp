#include "Game.h"

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

EGameState SGame::GetState() const
{
    return state_;
}

void SGame::SetState(const EGameState state)
{
    if (state_ == state)
    {
        return;
    }

    state_ = state;
    eventStateChanged_.Broadcast(state_);
}

KtEvent<EGameState>& SGame::EventStateChanged()
{
    return eventStateChanged_;
}
