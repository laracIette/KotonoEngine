#include "ObjectManager.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Window.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ModelManager.h>
#include <kotono_framework/Path.h>
#include "Object.h"
#include "SceneMeshObject.h"
#include "SceneMeshComponent.h"
#include "Scene.h"
#include "Camera.h"
#include "Engine.h"
#include "Visualizer.h"
#include "Interface.h"

static TCamera* Camera = nullptr;

void SObjectManager::Init()
{
	Framework.GetInputManager().GetKeyboard()
		.GetEvent(KT_KEY_ESCAPE, KT_INPUT_STATE_PRESSED)
		.AddListener(this, &SObjectManager::Quit);

	
	auto* shader3D = Framework.GetShaderManager().Get(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader3D.ktshader)");
	shader3D->SetName("3D Shader");

	auto* model1 = Framework.GetModelManager().Get(Framework.GetPath().GetSolutionPath() / R"(assets\models\viking_room.obj)");
	auto* model2 = Framework.GetModelManager().Get(Framework.GetPath().GetSolutionPath() / R"(assets\models\SM_Column_low.fbx)");

	auto* interface = Create<KInterface>();

	{
		auto* scene = Create<KScene>();
		scene->SetPath(Framework.GetPath().GetSolutionPath() / R"(assets\objects\scene.KScene)");
		scene->ListenEvent(Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_S, KT_INPUT_STATE_PRESSED), &KScene::Reload);
	}
	{
		auto* mesh1 = Create<TSceneMeshObject>();
		mesh1->GetMeshComponent()->SetShader(shader3D);
		mesh1->GetMeshComponent()->SetModel(model1);
		mesh1->GetTransform().SetRelativePosition(glm::vec3(-1.0f, 0.0f, 0.0f));

		auto* mesh2 = Create<TSceneMeshObject>();
		mesh2->GetMeshComponent()->SetShader(shader3D);
		mesh2->GetMeshComponent()->SetModel(model2);
		mesh2->GetTransform().SetRelativePosition(glm::vec3(1.0f, 0.0f, 0.0f));
		mesh2->GetTransform().SetRelativeScale(glm::vec3(0.2f));
		mesh2->SetParent(mesh1, ECoordinateSpace::World);
	}

	Camera = Create<TCamera>();
}

void SObjectManager::Update()
{
	InitObjects();
	Camera->Use();
	UpdateObjects();
	DrawObjects();
	DeleteObjects();
}

void SObjectManager::Cleanup()
{
	objects_.merge(inits_);
	for (auto* object : objects_)
	{
		object->Cleanup();
	}
	for (auto* object : objects_)
	{
		delete object;
	}
	objects_.clear();
	typeRegistry_.clear();
}

void SObjectManager::Register(KObject* object)
{
	KT_DEBUG_LOG(KT_LOG_IMPORTANCE_LEVEL_LOW, "creating object of type '%s'", object->GetTypeName().c_str());
	object->Construct();
	inits_.insert(object);
	typeRegistry_[typeid(*object)].insert(object);
}

KtEvent<>& SObjectManager::GetEventDrawSceneObjects()
{
	return eventDrawSceneObjects_;
}

KtEvent<>& SObjectManager::GetEventDrawSceneObjectWireframes()
{
	return eventDrawSceneObjectWireframes_;
}

KtEvent<>& SObjectManager::GetEventDrawInterfaceObjects()
{
	return eventDrawInterfaceObjects_;
}

KtEvent<>& SObjectManager::GetEventDrawInterfaceObjectBounds()
{
	return eventDrawInterfaceObjectBounds_;
}

KtEvent<>& SObjectManager::GetEventDrawInterfaceObjectWireframes()
{
	return eventDrawInterfaceObjectWireframes_;
}

void SObjectManager::Quit()
{
	Framework.GetWindow().SetShouldClose(true);
}

void SObjectManager::InitObjects()
{
	if (inits_.empty())
	{
		return;
	}

	for (auto* object : inits_)
	{
		object->Init();
	}
	objects_.merge(inits_);
}

void SObjectManager::UpdateObjects()
{
	for (auto* object : objects_)
	{
		object->Update();
	}
}

void SObjectManager::DeleteObjects()
{
	std::unordered_set<KObject*> deleteObjects;
	for (auto it = objects_.begin(); it != objects_.end();)
	{
		auto* object = *it;
		if (object->GetIsDelete())
		{
			KT_DEBUG_LOG(KT_LOG_IMPORTANCE_LEVEL_LOW, "deleting object '%s'", object->GetName().c_str());
			object->Cleanup();
			
			it = objects_.erase(it);  // Erase the object and move the iterator to the next element
			
			deleteObjects.insert(object);
		}
		else
		{
			++it;  // Only increment if not deleting
		}
	}
	for (auto* object : deleteObjects)
	{
		typeRegistry_[typeid(*object)].erase(object);
		delete object;
	}
}

void SObjectManager::DrawObjects()
{
	if (Engine.GetVisualizer().GetIsFieldVisible(EVisualizationField::SceneObject))
	{
		eventDrawSceneObjects_.Broadcast();
	}
	if (Engine.GetVisualizer().GetIsFieldVisible(EVisualizationField::SceneObjectWireframe))
	{
		eventDrawSceneObjectWireframes_.Broadcast();
	}
	if (Engine.GetVisualizer().GetIsFieldVisible(EVisualizationField::InterfaceObject))
	{
		eventDrawInterfaceObjects_.Broadcast();
	}
	if (Engine.GetVisualizer().GetIsFieldVisible(EVisualizationField::InterfaceObjectBounds))
	{
		eventDrawInterfaceObjectBounds_.Broadcast();
	}
	if (Engine.GetVisualizer().GetIsFieldVisible(EVisualizationField::InterfaceObjectWireframe))
	{
		eventDrawInterfaceObjectWireframes_.Broadcast();
	}
}
