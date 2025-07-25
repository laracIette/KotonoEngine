#include "SceneMeshObject.h"
#include "SceneMeshComponent.h"

TSceneMeshObject::TSceneMeshObject() :
	Base()
{
	meshComponent_ = AddComponent<KSceneMeshComponent>();
}

KSceneMeshComponent* TSceneMeshObject::GetMeshComponent() const
{
	return meshComponent_;
}
