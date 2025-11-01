#pragma once
#include "GameState.h"
#include <kotono_framework/Event.h>
#include "Ptr.h"
class KScene;
class SGame final
{
	friend class STimeManager;

private:
	void Update();

public:
	bool IsPlaying() const;
	bool IsPaused() const;
	bool IsStopped() const;

	void Play();
	void Pause();
	void Stop();

	void OpenScene(const UPtr<KScene>& scene);

	EGameState GetState() const;
	void SetState(const EGameState state);

	KtEvent<EGameState>& EventStateChanged();

private:
	EGameState state_;
	KtEvent<EGameState> eventStateChanged_;
	UPtr<KScene> scene_;
};

