#include "SceneMeshObject.h"
#include "SceneMeshComponent.h"

void TSceneMeshObject::Construct()
{
	Base::Construct();

	mesh_ = AddComponent<KSceneMeshComponent>();
}
