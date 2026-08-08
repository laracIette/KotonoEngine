#pragma once
#include "GameState.h"
#include <kotono_common/Event.h>
#include <kotono_object/Ptr.h>
class KScene;
class GGame final
{
	friend class GTimeManager;

public:
	void Init();
	void Update(const float deltaTime);
	void Cleanup();

	bool IsPlaying() const;
	bool IsPaused() const;
	bool IsStopped() const;

	void Play();
	void Pause();
	void Stop();

	EGameState GetState() const;
	UEvent<EGameState>& GetEventStateChanged();
	const UPtr<KScene>& GetOpenedScene() const;

	void SetState(const EGameState state);

	void OpenScene(const UPtr<KScene>& scene);

private:
	UPtr<KScene> GetStartupScene() const;
	void LoadScene();

	void AddTestSceneObject() const;

	void OnKeySPressed() const;

private:
	EGameState state_;
	UEvent<EGameState> eventStateChanged_;

	UPtr<KScene> scene_;
};

inline GGame Game;
