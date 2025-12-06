#include "Scene.h"
#include "Engine.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "SceneMeshComponent.h"
#include "SceneMeshObject.h"
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/ModelManager.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/Model.h>

KScene::KScene(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
	auto* shader3D{ ShaderManager.Get(::Path.Framework() / R"(shaders\shader3D.ktshader)") };
	shader3D->SetName("3D Shader");

	auto* model1{ ModelManager.Get(::Path.Framework() / R"(assets\models\viking_room.obj)") };
	auto* model2{ ModelManager.Get(::Path.Framework() / R"(assets\models\SM_Column_low.fbx)") };

	UPtr mesh1{ Engine.ObjectManager().Create<TSceneMeshObject>() };
	mesh1->GetMeshComponent()->SetShader(shader3D);
	mesh1->GetMeshComponent()->SetModel(model1);
	mesh1->RootComponent()->SetSpawnPosition({ -1.0f, 0.0f, 0.0f });

	UPtr mesh2{ Engine.ObjectManager().Create<TSceneMeshObject>() };
	mesh2->GetMeshComponent()->SetShader(shader3D);
	mesh2->GetMeshComponent()->SetModel(model2);
	mesh2->RootComponent()->SetSpawnPosition({ 1.0f, 0.0f, 0.0f });
	mesh2->RootComponent()->SetSpawnScale({ 0.2f, 0.2f, 0.2f });
	mesh2->SetParent(mesh1, ECoordinateSpace::World);

	sceneObjects_.Append({ mesh1, mesh2 });
}

void KScene::UpdateSceneObjects()
{
	for (const auto& sceneObject : sceneObjects_)
	{
		if (sceneObject->GetCanUpdate())
		{
			sceneObject->Update();
		}

		sceneObject->UpdateSceneComponents();
	}
}

void KScene::Load()
{
	Deserialize();
}

void KScene::Unload()
{
	for (const auto& sceneObject : sceneObjects_)
	{
		sceneObject->Delete();
	}
	sceneObjects_.Clear();
}

void KScene::Reload()
{
	Unload();
	Load();
}

void KScene::Add(const UPtr<TSceneObject>& sceneObject)
{
	sceneObjects_.Add(sceneObject);
}

void KScene::Remove(const UPtr<TSceneObject>& sceneObject)
{
	sceneObjects_.Remove(sceneObject);
}

void KScene::SpawnSceneObjects() const
{
	for (const auto& sceneObject : sceneObjects_)
	{
		sceneObject->RootComponent()->Spawn();
	}
}
