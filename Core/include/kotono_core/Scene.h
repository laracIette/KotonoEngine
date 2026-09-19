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
private:
	using SceneObject = UPtr<TSceneObject>;

public:
	explicit UScene(UPath const& path);
	~UScene();

	void Update(f32 deltaTime);

	void Add(SceneObject const& sceneObject);
	void Remove(SceneObject const& sceneObject);

	void SpawnSceneObjects();
	void AddSpawnedSceneObject(SceneObject const& sceneObject);

	auto GetSceneObjects() const -> std::span<SceneObject const>;

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const;

	void PlayGame();
	void PauseGame();
	void StopGame();

	void SelectObject(SceneObject const& sceneObject);

	auto GetAudioContext() -> UAudioContext& { return audioContext_; }

	auto GetEventSceneObjectsUpdated() -> UEvent<USet<SceneObject>>& { return eventSceneObjectsUpdated_; }
	auto GetEventSelectedObjectChanged() -> UEvent<SceneObject>& { return eventSelectedObjectChanged_; }
	auto GetSelectedObject() const -> SceneObject const& { return selectedObject_; }

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

	auto TrySetState(EGameState gameState) -> b8;

private:
	UAudioContext audioContext_;

	USet<SceneObject> sceneObjects_;
	USet<SceneObject> spawnedSceneObjects_;

	UEvent<USet<SceneObject>> eventSceneObjectsUpdated_;
	UEvent<SceneObject> eventSelectedObjectChanged_;
	SceneObject selectedObject_;

	UNotify<EGameState> gameState_;
	UNotify<f32> timeScale_;

	f32 deltaTime_;
	f32 now_;
};
