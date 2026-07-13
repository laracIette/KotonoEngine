#pragma once
#include "GameState.h"
#include <kotono_common/Event.h>
#include <kotono_object/Ptr.h>
class KScene;
class GGame final
{
	friend class GCore;
	friend class GTimeManager;

private:
	void Init();
	void Update(const float deltaTime);
	void Cleanup();

public:
	bool IsPlaying() const;
	bool IsPaused() const;
	bool IsStopped() const;

	void Play();
	void Pause();
	void Stop();

	EGameState GetState() const;
	void SetState(const EGameState state);

	UEvent<EGameState>& EventStateChanged();

public:
	void OpenScene(const UPtr<KScene>& scene);

	void OpenStartupScene();

	const UPtr<KScene>& GetOpenedScene() const;

private:
	void OnKeySPressed() const;

private:
	EGameState state_;
	UEvent<EGameState> eventStateChanged_;

	UPtr<KScene> scene_;
};

inline GGame Game;
