#include "ObjectManager.h"
#include <kotono_framework/Framework.h>
#include "Mesh.h"
#include "Image.h"
#include "Scene.h"
#include "Camera.h"

static TCamera* camera = nullptr;

void KObjectManager::Init()
{
	Framework.GetInputManager().GetKeyboard()
		.GetEvent(KT_KEY_ESCAPE, KT_INPUT_STATE_PRESSED)
		.AddListener(this, &KObjectManager::Quit);

	auto* shader2D = Framework.GetShaderManager().Create(
		Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2D.ktshader)"
	);
	auto* shader3D = Framework.GetShaderManager().Create(
		Framework.GetPath().GetFrameworkPath() / R"(shaders\shader3D.ktshader)"
	);
	shader2D->SetName("2D Shader");
	shader3D->SetName("3D Shader");

	auto* model1 = Framework.GetModelManager().Create(
		Framework.GetPath().GetSolutionPath() / (R"(assets\models\viking_room.obj)")
	);
	auto* model2 = Framework.GetModelManager().Create(
		Framework.GetPath().GetSolutionPath() / R"(assets\models\SM_Column_low.fbx)"
	);

	auto* scene = Create<OScene>();
	scene->SetPath(Framework.GetPath().GetSolutionPath() / R"(assets\objects\scene.oscene)");
#if false
	TMesh* mesh1 = Create<TMesh>();
	TMesh* mesh2 = Create<TMesh>();
	RImage* image1 = Create<RImage>();
	
	mesh1->SetShader(shader3D);
	mesh1->SetModel(model1);
	
	mesh2->SetShader(shader3D);
	mesh2->SetModel(model2);
	mesh2->GetTransform().SetRelativeLocation(glm::vec3(2.0f, 0.0f, 0.0f));
	mesh2->GetTransform().SetRelativeScale(glm::vec3(0.2f));
	
	image1->SetShader(shader2D);
	image1->GetRect().SetBaseSize(glm::uvec2(1024, 1024));
	image1->GetRect().SetRelativeScale(glm::vec2(0.5f));

	scene->Add(mesh1);
	scene->Add(mesh2);

	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_S, KT_INPUT_STATE_PRESSED).AddListener(
		scene, &OScene::Serialize
	);
#else
	Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_S, KT_INPUT_STATE_PRESSED).AddListener(
		scene, &OScene::Reload
	);
#endif
	camera = Create<TCamera>();
}

void KObjectManager::Update()
{
	InitObjects();
	camera->Use();
	UpdateObjects();
	DeleteObjects();
}

void KObjectManager::Cleanup()
{
	for (auto* object : _objects)
	{
		object->Cleanup();
		delete object;
	}
}

void KObjectManager::Quit()
{
	Framework.GetWindow().Close();
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
	for (auto it = _objects.begin(); it != _objects.end();)
	{
		auto* object = *it;
		if (object->GetIsDelete())
		{
			KT_DEBUG_LOG("deleting object '%s'", object->GetName().c_str());
			object->Cleanup();
			delete object;    

			
			it = _objects.erase(it);  // Erase the object and move the iterator to the next element
		}
		else
		{
			++it;  // Only increment if not deleting
		}
	}
}

void KObjectManager::Create(OObject* object)
{
	KT_DEBUG_LOG("creating object of type '%s'", object->GetTypeName().c_str());
	_objects.insert(object);
	_inits.insert(object);
}
