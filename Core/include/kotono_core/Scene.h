#pragma once
#include "GameState.h"
#include "Ptr.h"
#include <kotono_audio/AudioContext.h>
#include <kotono_common/Event.h>
#include <kotono_common/Notify.h>
#include <kotono_common/Path.h>
#include <kotono_common/Set.h>
#include <span>
struct USceneRenderGraph;
class TSceneObject;
class UScene final
{
public:
	explicit UScene(UPath const& path);
	~UScene();

	void Update(f32 deltaTime);

	void Add(UPtr<TSceneObject> const& sceneObject);
	void Remove(UPtr<TSceneObject> const& sceneObject);

	void SpawnSceneObjects();
	void AddSpawnedSceneObject(UPtr<TSceneObject> const& sceneObject);

	std::span<UPtr<TSceneObject> const> GetSceneObjects() const;

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const;

	void PlayGame();
	void PauseGame();
	void StopGame();

	auto GetAudioContext() -> UAudioContext& { return audioContext_; }

	auto GetEventSceneObjectsUpdated() -> UEvent<USet<UPtr<TSceneObject>>>& { return eventSceneObjectsUpdated_; }

	auto GetEventGameStateChanged() -> UEvent<EGameState>& { return gameState_.GetEventValueChanged(); }
	auto GetEventTimeScaleChanged() -> UEvent<f32>& { return timeScale_.GetEventValueChanged(); }

	auto GetIsGamePlaying() const -> b8 { return gameState_ == EGameState::Playing; }
	auto GetIsGamePaused() const -> b8 { return gameState_ == EGameState::Paused; }
	auto GetIsGameStopped() const -> b8 { return gameState_ == EGameState::Stopped; }

	auto GetDeltaTime() const -> f32 { return deltaTime_; }
	auto GetNow() const -> f32 { return now_; }
	auto GetTimeScale() const -> f32 { return timeScale_; }

	void SetTimeScale(f32 timeScale) { timeScale_ = timeScale; }

private:
	void UpdateSceneObjects(f32 deltaTime) const;

	b8 TrySetState(EGameState gameState);

private:
	UAudioContext audioContext_;

	USet<UPtr<TSceneObject>> sceneObjects_;
	USet<UPtr<TSceneObject>> spawnedSceneObjects_;

	UEvent<USet<UPtr<TSceneObject>>> eventSceneObjectsUpdated_;

	UNotify<EGameState> gameState_;
	UNotify<f32> timeScale_;

	f32 deltaTime_;
	f32 now_;
};
