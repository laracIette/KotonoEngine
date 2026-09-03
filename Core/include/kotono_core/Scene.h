#pragma once
#include "GameState.h"
#include "Ptr.h"
#include <kotono_common/Event.h>
#include <kotono_common/Path.h>
#include <kotono_common/Set.h>
#include <kotono_timing/TimeContext.h>
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

	b8 GetIsGamePlaying() const;
	b8 GetIsGamePaused() const;
	b8 GetIsGameStopped() const;

	UEvent<EGameState>& GetEventGameStateUpdated();

	UTimeContext const& GetGameTime() const;

private:
	void UpdateSceneObjects(f32 deltaTime) const;

	b8 TrySetState(EGameState gameState);

private:
	USet<UPtr<TSceneObject>> sceneObjects_;
	USet<UPtr<TSceneObject>> spawnedSceneObjects_;

	UEvent<> eventSceneObjectsUpdated_;
	UEvent<EGameState> eventGameStateUpdated_;

	EGameState gameState_;
	UTimeContext gameTime_;
};
