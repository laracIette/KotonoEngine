#include "MeshSceneObject.h"
#include "Engine.h"
#include "ObjectManager.h"
#include "MeshComponent.h"

void TMeshSceneObject::Construct()
{
	Base::Construct();

	mesh_ = Engine.GetObjectManager().Create<KMeshComponent>();
	AddComponent(mesh_);
}
