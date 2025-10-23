#pragma once
#include "SceneObject.h"

class KSceneMeshComponent;

class TSceneMeshObject : public TSceneObject
{
	BASECLASS(TSceneObject)

public:
	TSceneMeshObject(UPtrOwnerBase* ptrOwner);

	const UPtr<KSceneMeshComponent>& GetMeshComponent() const;

private:
	UPtr<KSceneMeshComponent> meshComponent_;
};

