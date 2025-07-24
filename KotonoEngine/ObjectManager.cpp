#include "ObjectManager.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Context.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/Window.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ModelManager.h>
#include <kotono_framework/Path.h>
#include "log.h"
#include "Object.h"
#include "SceneMeshObject.h"
#include "SceneMeshComponent.h"
#include "Scene.h"
#include "Camera.h"
#include "Engine.h"
#include "Timer.h"
#include "Time.h"

#define KT_LOG_IMPORTANCE_LEVEL_OBJECT KT_LOG_IMPORTANCE_LEVEL_NONE
#undef interface

void SObjectManager::Init()
{
	Framework.GetInputManager().GetKeyboard()
		.GetEvent(KT_KEY_ESCAPE, KT_INPUT_STATE_PRESSED)
		.AddListener(KtDelegate(this, &SObjectManager::Quit));
	
	auto* shader3D = Framework.GetShaderManager().Get(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader3D.ktshader)");
	shader3D->SetName("3D Shader");

	auto* model1 = Framework.GetModelManager().Get(Framework.GetPath().GetSolutionPath() / R"(assets\models\viking_room.obj)");
	auto* model2 = Framework.GetModelManager().Get(Framework.GetPath().GetSolutionPath() / R"(assets\models\SM_Column_low.fbx)");

	/*{
		auto* scene = Create<KScene>();
		scene->SetPath(Framework.GetPath().GetSolutionPath() / R"(assets\objects\scene.KScene)");
		scene->ListenEvent(Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_S, KT_INPUT_STATE_PRESSED), 
			KtDelegate(scene, &KScene::Reload));
	}*/
	{
		auto* mesh1 = Create<TSceneMeshObject>();
		mesh1->GetMeshComponent()->SetShader(shader3D);
		mesh1->GetMeshComponent()->SetModel(model1);
		mesh1->GetRootComponent()->SetRelativePosition(glm::vec3(-1.0f, 0.0f, 0.0f));

		auto* mesh2 = Create<TSceneMeshObject>();
		mesh2->GetMeshComponent()->SetShader(shader3D);
		mesh2->GetMeshComponent()->SetModel(model2);
		mesh2->GetRootComponent()->SetRelativePosition(glm::vec3(1.0f, 0.0f, 0.0f));
		mesh2->GetRootComponent()->SetRelativeScale(glm::vec3(0.2f));
		mesh2->SetParent(mesh1, ECoordinateSpace::World);
	}

	auto* camera = Create<TCamera>();
	camera->Use();

	drawTimer_ = Create<KTimer>();
	drawTimer_->SetDuration(1.0f / 120.0f);
	drawTimer_->SetIsRepeat(true);
	drawTimer_->GetEventCompleted().AddListener(KtDelegate(this, &SObjectManager::SubmitDrawObjects));
	drawTimer_->Start();

	auto* logUPSTimer = Create<KTimer>();
	logUPSTimer->SetDuration(1.0f);
	logUPSTimer->SetIsRepeat(true);
	logUPSTimer->GetEventCompleted().AddListener(KtDelegate(this, &SObjectManager::LogUPS));
	logUPSTimer->Start();
}

void SObjectManager::Update()
{
	InitObjects(); // todo: maybe put after delete
	UpdateObjects();
	DeleteObjects();
	if (canDraw_)
	{
		canDraw_ = false;
		Framework.GetRenderer().DrawFrame();
	}

	updateTimeIndex_ = (updateTimeIndex_ + 1) % updateTimes_.size();
	updateTimesSum_ -= updateTimes_[updateTimeIndex_];
	updateTimes_[updateTimeIndex_] = Engine.GetTime().GetDelta();
	updateTimesSum_ += updateTimes_[updateTimeIndex_];
}

void SObjectManager::Cleanup()
{
	objects_.Merge(inits_);
	for (auto* object : objects_)
	{
		object->Cleanup();
	}
	for (auto* object : objects_)
	{
		KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "DEL object %s", object->GetName().c_str());
		delete object;
	}
	objects_.Clear();
	typeRegistry_.clear();
}

void SObjectManager::Register(KObject* object)
{
	object->SetName(std::format("{}_{}", object->GetTypeName(), static_cast<std::string>(object->GetGuid())));
	KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "REG object %s", object->GetName().c_str());
	inits_.Add(object);
	typeRegistry_[typeid(*object)].insert(object);
}

void SObjectManager::Delete(KObject* object)
{
	deletes_.Add(object);
}

void SObjectManager::Quit()
{
	Framework.GetWindow().SetShouldClose(true);
}

void SObjectManager::InitObjects()
{
	if (inits_.Empty())
	{
		return;
	}

	KtPool<KObject*> inits{};
	inits.Merge(inits_);

	for (auto* object : inits)
	{
		object->Init();
		objects_.Add(object);
		object->objectIndex_ = objects_.LastIndex();
	}

	KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "object count %llu", objects_.Size());

	inits.Clear();
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
	if (deletes_.Empty())
	{
		return;
	}

	KtPool<KObject*> deletes{};
	deletes.Merge(deletes_);

	for (auto* object : deletes)
	{
		object->Cleanup();

		const size_t index = object->objectIndex_;
		objects_.RemoveAt(index);
		if (index < objects_.Size())
		{
			objects_[index]->objectIndex_ = index;
		}
	}
	for (auto* object : deletes)
	{
		KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "DEL object %s", object->GetName().c_str());
		typeRegistry_[typeid(*object)].erase(object);
		delete object;
	}
}

void SObjectManager::SubmitDrawObjects()
{
	canDraw_ = true;
}

const float SObjectManager::GetAverageUpdateTime() const
{
	return updateTimesSum_ / updateTimes_.size();
}

void SObjectManager::LogUPS()
{
	KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_HIGH, "%.2f ups", 1.0f / GetAverageUpdateTime());
}
