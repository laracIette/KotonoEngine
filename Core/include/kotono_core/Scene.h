#pragma once
#include "GameState.h"
#include "Ptr.h"
#include <kotono_common/Event.h>
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

	UEvent<>& GetEventSceneObjectsUpdated();

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const;

	void PlayGame();
	void PauseGame();
	void StopGame();

	UEvent<EGameState>& GetEventGameStateUpdated() { return eventGameStateUpdated_; }

	b8 GetIsGamePlaying() const { return gameState_ == EGameState::Playing; }
	b8 GetIsGamePaused() const { return gameState_ == EGameState::Paused; }
	b8 GetIsGameStopped() const { return gameState_ == EGameState::Stopped; }

	f32 GetDeltaTime() const { return deltaTime_; }
	f32 GetNow() const { return now_; }
	f32 GetTimeScale() const { return timeScale_; }

	void SetTimeScale(f32 timeScale) { timeScale_ = timeScale; }

private:
	void UpdateSceneObjects(f32 deltaTime) const;

	b8 TrySetState(EGameState gameState);

private:
	USet<UPtr<TSceneObject>> sceneObjects_;
	USet<UPtr<TSceneObject>> spawnedSceneObjects_;

	UEvent<> eventSceneObjectsUpdated_;
	UEvent<EGameState> eventGameStateUpdated_;

	EGameState gameState_;

	f32 deltaTime_;
	f32 now_;
	f32 timeScale_;
};
