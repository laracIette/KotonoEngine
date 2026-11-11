#pragma once
#include "GameState.h"
#include "SceneObject/SceneObject.h"
#include <AudioContext/AudioContext.h>
#include <Clamped.h>
#include <Event/Event.h>
#include <Path/Path.h>
#include <Notify.h>
#include <Containers/Set.h>
#include <span>

enum class ESceneVisibility : u32;
struct USceneRenderGraph;

class UScene final
{
public:
	using TimeScaleRange = UClamped<f32, 0.1f, 10.0f>;

private:
	using SceneObjectPtr = UObjectPtr<TSceneObject>;
	using SceneObjectSet = USet<SceneObjectPtr>;

public:
	explicit UScene(UPath const& path);
	~UScene();

	void Update(f32 deltaTime);

	void Add(SceneObjectPtr const& sceneObject);
	void Remove(SceneObjectPtr const& sceneObject);

	void SpawnSceneObjects();
	void DespawnSceneObjects();

	void Serialize() const;
	void Deserialize();

	void PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph, ESceneVisibility visibility) const;

	void PlayGame();
	void PauseGame();
	void StopGame();

	void SelectObject(SceneObjectPtr const& sceneObject);
	void DeselectObject(SceneObjectPtr const& sceneObject);
    void DeselectAllObjects();

    auto IsObjectSelected(SceneObjectPtr const& sceneObject) const -> b8;

	auto GetRayIntersectSceneComponent(glm::vec3 const& start, glm::vec3 const& direction) const -> UObjectPtr<KSceneComponent>;

	auto GetAudioContext() -> UAudioContext& { return audioContext_; }

	auto GetSceneObjects() const -> std::span<SceneObjectPtr const> { return sceneObjects_; }
	auto GetSelectedObjects() const -> std::span<SceneObjectPtr const> { return selectedObjects_; }

	auto GetEventSceneObjectsUpdated() -> UEvent<SceneObjectSet>& { return eventSceneObjectsUpdated_; }
	auto GetEventSelectedObjectsUpdated() -> UEvent<SceneObjectSet>& { return eventSelectedObjectsUpdated_; }

	auto GetEventGameStateChanged() -> UEvent<EGameState>& { return gameState_.GetEventValueChanged(); }
	auto GetEventTimeScaleChanged() -> UEvent<f32>& { return timeScale_.GetEventValueChanged(); }

	auto GetIsGamePlaying() const -> b8 { return gameState_ == EGameState::Playing; }
	auto GetIsGamePaused() const -> b8 { return gameState_ == EGameState::Paused; }
	auto GetIsGameStopped() const -> b8 { return gameState_ == EGameState::Stopped; }

	auto GetDeltaTime() const -> f32 { return deltaTime_; }
	auto GetNow() const -> f32 { return now_; }
	auto GetTimeScale() const -> f32 { return timeScale_; }

	void SetTimeScale(TimeScaleRange timeScale) { timeScale_ = timeScale; }

private:
	void InitSceneObjects() const;
	void UpdateSceneObjects(f32 deltaTime) const;
	void DeleteSceneObjects();

	auto TrySetState(EGameState gameState) -> b8;

private:
	UPath path_;
	
	UAudioContext audioContext_;

	SceneObjectSet sceneObjects_;
	SceneObjectSet selectedObjects_;

	b8 areSceneObjectsSpawned_;

	UEvent<SceneObjectSet> eventSceneObjectsUpdated_;
	UEvent<SceneObjectSet> eventSelectedObjectsUpdated_;

	UNotify<EGameState> gameState_;
	UNotify<f32> timeScale_;

	f32 deltaTime_;
	f32 now_;
};
