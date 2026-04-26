#pragma once
#include "generated/Scene.generated.h"
#include <kotono_object/Object.h>
#include <kotono_common/Event.h>
#include <kotono_common/Pool.h>

class TSceneObject;

class KScene : public KObject
{
	GENERATED_KSCENE()

private:
	friend class SGame;

public:
	~KScene() override;

public:
	void Add(const UPtr<TSceneObject>& sceneObject);
	void Remove(const UPtr<TSceneObject>& sceneObject);

	void SpawnSceneObjects();

	const UPool<UPtr<TSceneObject>>& SceneObjects() const;

	UEvent<>& EventSceneObjectsUpdated();

private:
	void Update(const float deltaTime);

private:
	SERIALIZE UPool<UPtr<TSceneObject>> sceneObjects_;
	UEvent<> eventSceneObjectsUpdated_;
};
