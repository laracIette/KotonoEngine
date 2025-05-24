#include "SceneMeshObject.h"
#include "SceneMeshComponent.h"

void TSceneMeshObject::Construct()
{
	Base::Construct();

	meshComponent_ = AddComponent<KSceneMeshComponent>();
}

KSceneMeshComponent* TSceneMeshObject::GetMeshComponent() const
{
	return meshComponent_;
}
