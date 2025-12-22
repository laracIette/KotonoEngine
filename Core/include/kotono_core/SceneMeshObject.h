#pragma once
#include "generated/SceneMeshObject.generated.h"
#include "SceneObject.h"

class TSceneMeshObject : public TSceneObject
{
	GENERATED_TSCENEMESHOBJECT()

public:
	TSceneMeshObject(UPtrOwnerBase* ptrOwner);
};

