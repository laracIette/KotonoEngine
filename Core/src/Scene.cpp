#include "Scene.h"
#include "SceneObject.h"

void KScene::Cleanup()
{
	for (const auto& sceneObject : sceneObjects_)
	{
		if (sceneObject)
		{
			sceneObject->Delete();
		}
	}
	sceneObjects_.Clear();

	Base::Cleanup();
}

void KScene::UpdateSceneObjects()
{
	for (auto& sceneObject : sceneObjects_)
	{
		if (sceneObject->GetCanUpdate())
		{
			sceneObject->Update();
		}

		sceneObject->UpdateSceneComponents();
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

KtEvent<>& KScene::EventSceneObjectsUpdated()
{
	return eventSceneObjectsUpdated_;
}