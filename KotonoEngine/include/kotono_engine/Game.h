#pragma once
#include "GameState.h"
#include <kotono_framework/Event.h>
class SGame final
{
public:
	bool IsPlaying() const;
	bool IsPaused() const;
	bool IsStopped() const;

	void Play();
	void Pause();
	void Stop();

	EGameState GetState() const;
	void SetState(const EGameState state);

	KtEvent<EGameState>& EventStateChanged();

private:
	EGameState state_;
	KtEvent<EGameState> eventStateChanged_;
};

