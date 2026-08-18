#include "Scene.h"
#include "SceneObject.h"
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>
#include <vector>

UScene::UScene(UPath const& path)
	: gameState_{ EGameState::Stopped }
	, gameTime_{
		.frequency = 1.0f / 120.0f,
		.lastDelta = 0.0f,
		.currentDelta = 0.0f,
		.scale = 1.0f,
		.total = 0.0f,
		.state = ETimeContextState::Paused,
	}
{
	nlohmann::json json{};
	SSerializer::Deserialize(json, path);

	std::vector<UPtr<TSceneObject>> sceneObjects{};
	UDeserialize<decltype(sceneObjects)>{}(json["sceneObjects"], sceneObjects);

	for (auto const& sceneObject : sceneObjects)
	{
		if (sceneObject)
		{
			sceneObject->scene_ = this;
			sceneObjects_.Add(sceneObject);
		}
	}

	SpawnSceneObjects();
}

UScene::~UScene()
{
	for (auto const& sceneObject : sceneObjects_)
	{
		if (sceneObject)
		{
			sceneObject->Delete();
		}
	}
}

void UScene::Update(f32 deltaTime)
{
	if (gameTime_.Update(deltaTime))
	{
		UpdateSceneObjects(gameTime_.lastDelta);
	}
}

void UScene::Add(UPtr<TSceneObject> const& sceneObject)
{
	if (!sceneObject)
	{
		return;
	}

	sceneObjects_.Add(sceneObject);
	sceneObject->scene_ = this;
	eventSceneObjectsUpdated_.Broadcast();
}

void UScene::Remove(UPtr<TSceneObject> const& sceneObject)
{
	if (!sceneObject)
	{
		return;
	}

	sceneObjects_.Remove(sceneObject);
	sceneObject->scene_ = nullptr;
	eventSceneObjectsUpdated_.Broadcast();
}

void UScene::SpawnSceneObjects()
{
	for (auto const& sceneObject : sceneObjects_)
	{
		sceneObject->Spawn();
	}
}

void UScene::AddSpawnedSceneObject(UPtr<TSceneObject> const& sceneObject)
{
	if (!sceneObject)
	{
		return;
	}

	spawnedSceneObjects_.Add(sceneObject);
}

std::span<UPtr<TSceneObject> const> UScene::GetSceneObjects() const
{
	return sceneObjects_;
}

UEvent<>& UScene::GetEventSceneObjectsUpdated()
{
	return eventSceneObjectsUpdated_;
}

void UScene::PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph) const
{
	for (auto const& sceneObject : spawnedSceneObjects_)
	{
		if (sceneObject)
		{
			sceneObject->PopulateRenderGraph(sceneRenderGraph);
		}
	}
}

void UScene::PlayGame()
{
	if (TrySetState(EGameState::Playing))
	{
		gameTime_.state = ETimeContextState::Playing;
	}
}

void UScene::PauseGame()
{
	if (TrySetState(EGameState::Paused))
	{
		gameTime_.state = ETimeContextState::Paused;
	}
}

void UScene::StopGame()
{
	if (TrySetState(EGameState::Stopped))
	{
		gameTime_.total = 0.0f;
		gameTime_.currentDelta = 0.0f;
		gameTime_.state = ETimeContextState::Paused;
	}
}

b8 UScene::GetIsGamePlaying() const
{
	return gameState_ == EGameState::Playing;
}

b8 UScene::GetIsGamePaused() const
{
	return gameState_ == EGameState::Paused;
}

b8 UScene::GetIsGameStopped() const
{
	return gameState_ == EGameState::Stopped;
}

UEvent<EGameState>& UScene::GetEventGameStateUpdated()
{
	return eventGameStateUpdated_;
}

UTimeContext const& UScene::GetGameTime() const
{
	return gameTime_;
}

void UScene::UpdateSceneObjects(f32 deltaTime) const
{
	for (auto const& sceneObject : sceneObjects_)
	{
		if (sceneObject->GetCanUpdate())
		{
			sceneObject->Update(deltaTime);
		}

		if (!sceneObject->isInit_)
		{
			sceneObject->Init();
			sceneObject->isInit_ = true;
		}

		sceneObject->InitSceneComponents();
		sceneObject->UpdateSceneComponents(deltaTime);
	}
}

b8 UScene::TrySetState(EGameState gameState)
{
	if (gameState_ == gameState)
	{
		return false;
	}

	gameState_ = gameState;
	return true;
}
