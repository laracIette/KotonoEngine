#pragma once
#include "generated/Scene.generated.h"
#include "Object.h"
#include <kotono_framework/Pool.h>

class TSceneObject;

class KScene : public KObject
{
	GENERATED_KSCENE()

public:
	KScene(UPtrOwnerBase* ptrOwner);

public:
	void UpdateSceneObjects();

	void Load();
	void Unload();
	void Reload();

	void Add(const UPtr<TSceneObject>& sceneObject);
	void Remove(const UPtr<TSceneObject>& sceneObject);

	void SpawnSceneObjects() const;

private:
	SERIALIZE KtPool<UPtr<TSceneObject>> sceneObjects_;
};

