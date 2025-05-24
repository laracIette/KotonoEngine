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
	DeleteObjects();
	DrawObjects();
}

void SObjectManager::Cleanup()
{
	for (auto* object : _objects)
	{
		object->Cleanup();
	}
	for (auto* object : _objects)
	{
		delete object;
	}
	_objects.clear();
	_inits.clear();
	_typeRegistry.clear();
}

void SObjectManager::Register(KObject* object)
{
	object->Construct();
	KT_DEBUG_LOG(KT_LOG_IMPORTANCE_LEVEL_LOW, "creating object of type '%s'", object->GetTypeName().c_str());
	_inits.insert(object);
	_objects.insert(object);
	_typeRegistry[typeid(*object)].insert(object);
}

KtEvent<>& SObjectManager::GetEventDrawSceneObjects()
{
	return _eventDrawSceneObjects;
}

KtEvent<>& SObjectManager::GetEventDrawSceneObjectWireframes()
{
	return _eventDrawSceneObjectWireframes;
}

KtEvent<>& SObjectManager::GetEventDrawInterfaceObjects()
{
	return _eventDrawInterfaceObjects;
}

KtEvent<>& SObjectManager::GetEventDrawInterfaceObjectBounds()
{
	return _eventDrawInterfaceObjectBounds;
}

KtEvent<>& SObjectManager::GetEventDrawInterfaceObjectWireframes()
{
	return _eventDrawInterfaceObjectWireframes;
}

void SObjectManager::Quit()
{
	Framework.GetWindow().SetShouldClose(true);
}

void SObjectManager::InitObjects()
{
	for (auto* object : _inits)
	{
		object->Init();
	}
	_inits.clear();
}

void SObjectManager::UpdateObjects()
{
	for (auto* object : _objects)
	{
		object->Update();
	}
}

void SObjectManager::DeleteObjects()
{
	std::unordered_set<KObject*> deleteObjects;
	for (auto it = _objects.begin(); it != _objects.end();)
	{
		auto* object = *it;
		if (object->GetIsDelete())
		{
			KT_DEBUG_LOG(KT_LOG_IMPORTANCE_LEVEL_LOW, "deleting object '%s'", object->GetName().c_str());
			object->Cleanup();
			
			it = _objects.erase(it);  // Erase the object and move the iterator to the next element
			
			deleteObjects.insert(object);
		}
		else
		{
			++it;  // Only increment if not deleting
		}
	}
	for (auto* object : deleteObjects)
	{
		_typeRegistry[typeid(*object)].erase(object);
		delete object;
	}
}

void SObjectManager::DrawObjects()
{
	if (Engine.GetVisualizer().GetIsFieldVisible(EVisualizationField::SceneObject))
	{
		_eventDrawSceneObjects.Broadcast();
	}
	if (Engine.GetVisualizer().GetIsFieldVisible(EVisualizationField::SceneObjectWireframe))
	{
		_eventDrawSceneObjectWireframes.Broadcast();
	}
	if (Engine.GetVisualizer().GetIsFieldVisible(EVisualizationField::InterfaceObject))
	{
		_eventDrawInterfaceObjects.Broadcast();
	}
	if (Engine.GetVisualizer().GetIsFieldVisible(EVisualizationField::InterfaceObjectBounds))
	{
		_eventDrawInterfaceObjectBounds.Broadcast();
	}
	if (Engine.GetVisualizer().GetIsFieldVisible(EVisualizationField::InterfaceObjectWireframe))
	{
		_eventDrawInterfaceObjectWireframes.Broadcast();
	}
}
