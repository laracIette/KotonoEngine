#pragma once
#include "generated/SceneMeshObject.generated.h"
#include "SceneObject.h"

class KSceneMeshComponent;

class TSceneMeshObject : public TSceneObject
{
	GENERATED_TSCENEMESHOBJECT()

public:
	TSceneMeshObject(UPtrOwnerBase* ptrOwner);

	const UPtr<KSceneMeshComponent>& GetMeshComponent() const;

private:
	SERIALIZE UPtr<KSceneMeshComponent> meshComponent_;
};

