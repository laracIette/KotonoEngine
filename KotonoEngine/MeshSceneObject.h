#pragma once
#include "SceneObject.h"

class KMeshComponent;

class TMeshSceneObject : public TSceneObject
{
	BASECLASS(TSceneObject)

public:
	void Construct();

private:
	KMeshComponent* mesh_;
};

