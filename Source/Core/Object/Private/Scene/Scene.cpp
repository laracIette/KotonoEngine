#include "Scene.h"

#include "SceneObject.h"
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>
#include <vector>

UScene::UScene(UPath const& path)
	: gameState_{ EGameState::Stopped }
	, deltaTime_{ 0.0f }
	, now_{ 0.0f }
	, timeScale_{ 1.0f }
	, sceneObjects_{}
	, areSceneObjectsSpawned_{ false }
{
	audioContext_.Init();

	nlohmann::json json{};
	SSerializer::Deserialize(json, path);

	std::vector<SceneObject> sceneObjects{};
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
	for (auto const& sceneObject : USet<SceneObject>{ sceneObjects_ })
	{
		if (sceneObject)
		{
			sceneObject->Delete();
		}
	}

	audioContext_.Cleanup();
}

void UScene::Update(f32 deltaTime)
{
	if (gameState_ == EGameState::Playing)
	{
		deltaTime *= timeScale_;

		deltaTime_ = deltaTime;
		now_ += deltaTime;

		InitSceneObjects();
		UpdateSceneObjects(deltaTime);

		audioContext_.Update();
	}
}

void UScene::Add(SceneObject const& sceneObject)
{
	if (!sceneObject)
	{
		return;
	}

	sceneObjects_.Add(sceneObject);
	sceneObject->scene_ = this;

	if (areSceneObjectsSpawned_)
	{
		sceneObject->Spawn();
	}

	eventSceneObjectsUpdated_.Broadcast(sceneObjects_);
}

void UScene::Remove(SceneObject const& sceneObject)
{
	if (!sceneObject)
	{
		return;
	}

	if (areSceneObjectsSpawned_)
	{
		sceneObject->Despawn();
	}

	sceneObjects_.Remove(sceneObject);
	sceneObject->scene_ = nullptr;

	eventSceneObjectsUpdated_.Broadcast(sceneObjects_);
}

void UScene::SpawnSceneObjects()
{
	if (areSceneObjectsSpawned_)
	{
		return;
	}

	areSceneObjectsSpawned_ = true;

	for (auto const& sceneObject : sceneObjects_)
	{
		if (sceneObject)
		{
			sceneObject->Spawn();
		}
	}
}

void UScene::DespawnSceneObjects()
{
	if (!areSceneObjectsSpawned_)
	{
		return;
	}

	areSceneObjectsSpawned_ = false;

	for (auto const& sceneObject : sceneObjects_)
	{
		if (sceneObject)
		{
			sceneObject->Despawn();
		}
	}
}

auto UScene::GetSceneObjects() const -> std::span<SceneObject const>
{
	return sceneObjects_;
}

void UScene::PopulateRenderGraph(USceneRenderGraph& sceneRenderGraph, ESceneVisibility visibility) const
{
	for (auto const& sceneObject : sceneObjects_)
	{
		if (sceneObject)
		{
			sceneObject->PopulateRenderGraph(sceneRenderGraph, visibility);
		}
	}
}

void UScene::PlayGame()
{
	TrySetState(EGameState::Playing);
}

void UScene::PauseGame()
{
	TrySetState(EGameState::Paused);
}

void UScene::StopGame()
{
	if (TrySetState(EGameState::Stopped))
	{
		now_ = 0.0f;
	}
}

void UScene::SelectObject(SceneObject const& sceneObject)
{
	if (sceneObject == selectedObject_)
	{
		return;
	}

	selectedObject_ = sceneObject;
	eventSelectedObjectChanged_.Broadcast(sceneObject);
}

void UScene::InitSceneObjects() const
{
	for (auto const& sceneObject : sceneObjects_)
	{
		if (!sceneObject)
		{
			continue;
		}

		if (!sceneObject->isInit_)
		{
			sceneObject->Init();
			sceneObject->isInit_ = true;
		}

		sceneObject->InitSceneComponents();
	}
}

void UScene::UpdateSceneObjects(f32 deltaTime) const
{
	for (auto const& sceneObject : sceneObjects_)
	{	
		if (!sceneObject)
		{
			continue;
		}

		if (sceneObject && sceneObject->GetCanUpdate())
		{
			sceneObject->Update(deltaTime);
		}

		sceneObject->UpdateSceneComponents(deltaTime);
	}
}

auto UScene::TrySetState(EGameState gameState) -> b8
{
	if (gameState_ == gameState)
	{
		return false;
	}

	gameState_ = gameState;
	return true;
}
