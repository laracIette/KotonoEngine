#pragma once
#include "generated/Scene.generated.h"
#include "Object.h"
#include <kotono_common/Pool.h>

class TSceneObject;

class KScene : public KObject
{
	GENERATED_KSCENE()

public:
	KScene(UPtrOwnerBase* ptrOwner);

protected:
	void Cleanup() override;

public:
	void UpdateSceneObjects();

	void Add(const UPtr<TSceneObject>& sceneObject);
	void Remove(const UPtr<TSceneObject>& sceneObject);

	void SpawnSceneObjects();

	const KtPool<UPtr<TSceneObject>>& SceneObjects() const;

private:
	SERIALIZE KtPool<UPtr<TSceneObject>> sceneObjects_;
};

