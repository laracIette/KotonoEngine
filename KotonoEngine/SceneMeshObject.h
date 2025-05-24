#pragma once
#include "SceneObject.h"

class KSceneMeshComponent;

class TSceneMeshObject : public TSceneObject
{
	BASECLASS(TSceneObject)

public:
	void Construct() override;

	KSceneMeshComponent* GetMeshComponent() const;

private:
	KSceneMeshComponent* meshComponent_;
};

