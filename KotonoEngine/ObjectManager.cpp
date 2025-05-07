#include "ObjectManager.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Window.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ModelManager.h>
#include <kotono_framework/ImageTextureManager.h>
#include <kotono_framework/Path.h>
#include "Mesh.h"
#include "Image.h"
#include "Scene.h"
#include "Camera.h"
#include "InterfaceObjectStack.h"
#include "Engine.h"
#include "Visualizer.h"

static TCamera* Camera = nullptr;

void KObjectManager::Init()
{
	Framework.GetInputManager().GetKeyboard()
		.GetEvent(KT_KEY_ESCAPE, KT_INPUT_STATE_PRESSED)
		.AddListener(this, &KObjectManager::Quit);

	auto* shader2D = Framework.GetShaderManager().Create(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2D.ktshader)");
	auto* shader3D = Framework.GetShaderManager().Create(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader3D.ktshader)");
	shader2D->SetName("2D Shader");
	shader3D->SetName("3D Shader");

	auto* model1 = Framework.GetModelManager().Create(Framework.GetPath().GetSolutionPath() / R"(assets\models\viking_room.obj)");
	auto* model2 = Framework.GetModelManager().Create(Framework.GetPath().GetSolutionPath() / R"(assets\models\SM_Column_low.fbx)");

	auto* imageTexture1 = Framework.GetImageTextureManager().Create(Framework.GetPath().GetSolutionPath() / R"(assets\models\viking_room.png)");
	auto* imageTexture2 = Framework.GetImageTextureManager().Create(Framework.GetPath().GetSolutionPath() / R"(assets\textures\default_texture.jpg)");

	{
		auto* scene = Create<OScene>();
		scene->SetPath(Framework.GetPath().GetSolutionPath() / R"(assets\objects\scene.oscene)");
		scene->ListenEvent(Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_S, KT_INPUT_STATE_PRESSED), &OScene::Reload);
	}
	{
		constexpr bool HS = true;

		auto* image1 = Create<RImage>();
		image1->SetShader(shader2D);
		image1->SetImageTexture(imageTexture1);
		image1->GetRect().SetScreenSize(glm::vec2(1024.0f, 1024.0f));
		image1->GetRect().SetRelativeScale(glm::vec2(0.25f));
		//image1->GetRect().SetAnchor(EAnchor::TopLeft);

		auto* image2 = Create<RImage>();
		image2->SetShader(shader2D);
		image2->SetImageTexture(imageTexture2);
		image2->GetRect().SetScreenSize(glm::vec2(1024.0f, 1024.0f));
		image2->GetRect().SetRelativeScale(glm::vec2(0.10f));
		
		if (!HS)
		{
			image2->SetParent(image1, ECoordinateSpace::World);
		}
		else
		{
			auto* horizontalStack = Create<RHorizontalInterfaceObjectStack>();
			horizontalStack->SetItemSpacing(0.1f);
			horizontalStack->AddItem(image1);
			horizontalStack->AddItem(image2);
		}
	}
	{
		auto* mesh1 = Create<TMesh>();
		mesh1->SetShader(shader3D);
		mesh1->SetModel(model1);
		mesh1->GetTransform().SetRelativePosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	
		auto* mesh2 = Create<TMesh>();
		mesh2->SetShader(shader3D);
		mesh2->SetModel(model2);
		mesh2->GetTransform().SetRelativePosition(glm::vec3(1.0f, 0.0f, 0.0f));
		mesh2->GetTransform().SetRelativeScale(glm::vec3(0.2f));
		mesh2->SetParent(mesh1, ECoordinateSpace::World);
	}

	Camera = Create<TCamera>();
}

void KObjectManager::Update()
{
	InitObjects();
	Camera->Use();
	UpdateObjects();
	DeleteObjects();
	DrawObjects();
}

void KObjectManager::Cleanup()
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

KtEvent<>& KObjectManager::GetEventDrawSceneObjects()
{
	return _eventDrawSceneObjects;
}

KtEvent<>& KObjectManager::GetEventDrawSceneObjectWireframes()
{
	return _eventDrawSceneObjectWireframes;
}

KtEvent<>& KObjectManager::GetEventDrawInterfaceObjects()
{
	return _eventDrawInterfaceObjects;
}

KtEvent<>& KObjectManager::GetEventDrawInterfaceObjectBounds()
{
	return _eventDrawInterfaceObjectBounds;
}

KtEvent<>& KObjectManager::GetEventDrawInterfaceObjectWireframes()
{
	return _eventDrawInterfaceObjectWireframes;
}

void KObjectManager::Quit()
{
	Framework.GetWindow().SetShouldClose(true);
}

void KObjectManager::InitObjects()
{
	for (auto* object : _inits)
	{
		object->Init();
	}
	_inits.clear();
}

void KObjectManager::UpdateObjects()
{
	for (auto* object : _objects)
	{
		object->Update();
	}
}

void KObjectManager::DeleteObjects()
{
	std::unordered_set<OObject*> deleteObjects;
	for (auto it = _objects.begin(); it != _objects.end();)
	{
		auto* object = *it;
		if (object->GetIsDelete())
		{
			KT_DEBUG_LOG("deleting object '%s'", object->GetName().c_str());
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

void KObjectManager::DrawObjects()
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

void KObjectManager::Create(OObject* object)
{
	KT_DEBUG_LOG("creating object of type '%s'", object->GetTypeName().c_str());
	_objects.insert(object);
	_inits.insert(object);
	_typeRegistry[typeid(*object)].insert(object);
}
