#include "SceneMeshObject.h"
#include "SceneMeshComponent.h"
#include "Engine.h"
#include "ObjectManager.h"

TSceneMeshObject::TSceneMeshObject(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
	meshComponent_ = Engine.ObjectManager().Create<KSceneMeshComponent>(Ptr<TSceneMeshObject>());
	AddComponent(meshComponent_);
}

const UPtr<KSceneMeshComponent>& TSceneMeshObject::GetMeshComponent() const
{
	return meshComponent_;
}
