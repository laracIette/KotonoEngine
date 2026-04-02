#include "Scene.h"
#include "SceneObject.h"

KScene::~KScene()
{
	for (const auto& sceneObject : sceneObjects_)
	{
		if (sceneObject)
		{
			sceneObject->Delete();
		}
	}
}

void KScene::Update(const float deltaTime)
{
	for (auto& sceneObject : sceneObjects_)
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

void KScene::Add(const UPtr<TSceneObject>& sceneObject)
{
	sceneObjects_.Add(sceneObject);
	eventSceneObjectsUpdated_.Broadcast();
}

void KScene::Remove(const UPtr<TSceneObject>& sceneObject)
{
	sceneObjects_.Remove(sceneObject); 
	eventSceneObjectsUpdated_.Broadcast();
}

void KScene::SpawnSceneObjects()
{
	for (auto& sceneObject : sceneObjects_)
	{
		sceneObject->Spawn();
	}
}

const KtPool<UPtr<TSceneObject>>& KScene::SceneObjects() const
{
	return sceneObjects_;
}

UEvent<>& KScene::EventSceneObjectsUpdated()
{
	return eventSceneObjectsUpdated_;
}