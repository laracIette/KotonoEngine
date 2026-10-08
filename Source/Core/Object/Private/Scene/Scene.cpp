#include "Scene/Scene.h"

#include "SceneObject/SceneObject.h"
#include <Serializer/Serializer.h>
#include <nlohmann/json.hpp>
#include <vector>

UScene::UScene(UPath const& path)
	: path_{ path }
	, sceneObjects_{}
	, areSceneObjectsSpawned_{ false }
	, gameState_{ EGameState::Stopped }
	, timeScale_{ 1.0f }
	, deltaTime_{ 0.0f }
	, now_{ 0.0f }
{
	audioContext_.Init();

	Deserialize();

	SpawnSceneObjects();
}

UScene::~UScene()
{
	DeleteSceneObjects();

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

void UScene::Add(SceneObjectPtr const& sceneObject)
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

void UScene::Remove(SceneObjectPtr const& sceneObject)
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

void UScene::Serialize() const
{
	for (auto const& sceneObject : sceneObjects_)
	{
		if (sceneObject)
		{
			sceneObject->Serialize();
		}
	}
	
	nlohmann::json json{};
	USerialize<decltype(sceneObjects_)>{}(json["sceneObjects"], sceneObjects_);

	SSerializer::Serialize(json, path_);
}

void UScene::Deserialize()
{
	nlohmann::json json{};
	SSerializer::Deserialize(json, path_);
	UDeserialize<decltype(sceneObjects_)>{}(json["sceneObjects"], sceneObjects_);
	
	for (auto const& sceneObject : sceneObjects_)
	{
		if (sceneObject)
		{
			sceneObject->scene_ = this;
		}
	}
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
		
		DespawnSceneObjects();
		DeleteSceneObjects();
		
		Deserialize();
		SpawnSceneObjects();
	}
}

void UScene::SelectObject(SceneObjectPtr const& sceneObject)
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

void UScene::DeleteSceneObjects() const
{
	for (auto const& sceneObject : USet<SceneObjectPtr>{ sceneObjects_ })
	{
		if (sceneObject)
		{
			sceneObject->Delete();
		}
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
