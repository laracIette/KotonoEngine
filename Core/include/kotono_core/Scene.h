#pragma once
#include "generated/Scene.generated.h"
#include "Object.h"
#include <kotono_common/Pool.h>

class TSceneObject;

class KScene : public KObject
{
	GENERATED_KSCENE()

private:
	friend class SGame;

protected:
	void Cleanup() override;

public:
	void Add(const UPtr<TSceneObject>& sceneObject);
	void Remove(const UPtr<TSceneObject>& sceneObject);

	void SpawnSceneObjects();

	const KtPool<UPtr<TSceneObject>>& SceneObjects() const;

	KtEvent<>& EventSceneObjectsUpdated();

private:
	void Update(const float deltaTime);

private:
	SERIALIZE KtPool<UPtr<TSceneObject>> sceneObjects_;
	KtEvent<> eventSceneObjectsUpdated_;

};

