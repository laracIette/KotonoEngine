#include "Scene.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "SceneMeshComponent.h"
#include "SceneMeshObject.h"
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/ModelManager.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/Model.h>
#include <kotono_common/log.h>

KScene::KScene(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
	//auto* shader3D{ ShaderManager.Get(KtPath::Graphics() / "shaders" / "shader3D.ktshader") };
	//shader3D->SetName("3D Shader");
	//
	//auto* model1{ ModelManager.Get(KtPath::Graphics() / "assets" / "models" / "viking_room.obj") };
	//auto* model2{ ModelManager.Get(KtPath::Graphics() / "assets" / "models" / "SM_Column_low.fbx") };
	//
	//UPtr mesh1{ ObjectManager.Create<TSceneObject>() };
	//UPtr rootComponent1{ ObjectManager.Create<KSceneComponent>() };
	//UPtr meshComponent1{ ObjectManager.Create<KSceneMeshComponent>() };
	//
	//rootComponent1->SetOwner(mesh1);
	//meshComponent1->SetOwner(mesh1);
	//
	//rootComponent1->SetRelativePosition({ -1.0f, 0.0f, 0.0f });
	//meshComponent1->SetShader(shader3D);
	//meshComponent1->SetModel(model1);
	//meshComponent1->SetParent(rootComponent1, ECoordinateSpace::Relative);
	//
	//sceneObjects_.Append({ mesh1 });
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
		if (sceneObject && sceneObject->RootComponent())
		{
			sceneObject->RootComponent()->Spawn();
		}
	}
}
