#include "ObjectManager.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Context.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/Window.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/ModelManager.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Stopwatch.h>
#include "log.h"
#include "Object.h"
#include "SceneMeshObject.h"
#include "SceneMeshComponent.h"
#include "Scene.h"
#include "Camera.h"
#include "Engine.h"
#include "Timer.h"
#include "TimeManager.h"

#define KT_LOG_IMPORTANCE_LEVEL_OBJECT KT_LOG_IMPORTANCE_LEVEL_NONE

void SObjectManager::Init()
{
	Framework.GetInputManager().GetKeyboard()
		.GetEvent(KT_KEY_ESCAPE, KT_INPUT_STATE_PRESSED)
		.AddListener(KtDelegate(this, &SObjectManager::Quit));
	
	auto* shader3D{ Framework.GetShaderManager().Get(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader3D.ktshader)") };
	shader3D->SetName("3D Shader");

	auto* model1{ Framework.GetModelManager().Get(Framework.GetPath().GetSolutionPath() / R"(assets\models\viking_room.obj)") };
	auto* model2{ Framework.GetModelManager().Get(Framework.GetPath().GetSolutionPath() / R"(assets\models\SM_Column_low.fbx)") };

	/*{
		auto* scene{ Create<KScene>() };
		scene->SetPath(Framework.GetPath().GetSolutionPath() / R"(assets\objects\scene.KScene)");
		scene->ListenEvent(Framework.GetInputManager().GetKeyboard().GetEvent(KT_KEY_S, KT_INPUT_STATE_PRESSED), 
			KtDelegate(scene, &KScene::Reload));
	}*/
	{
		auto* mesh1{ Create<TSceneMeshObject>() };
		mesh1->GetMeshComponent()->SetShader(shader3D);
		mesh1->GetMeshComponent()->SetModel(model1);
		mesh1->GetRootComponent()->SetRelativePosition(glm::vec3(-1.0f, 0.0f, 0.0f));

		auto* mesh2{ Create<TSceneMeshObject>() };
		mesh2->GetMeshComponent()->SetShader(shader3D);
		mesh2->GetMeshComponent()->SetModel(model2);
		mesh2->GetRootComponent()->SetRelativePosition(glm::vec3(1.0f, 0.0f, 0.0f));
		mesh2->GetRootComponent()->SetRelativeScale(glm::vec3(0.2f));
		mesh2->SetParent(mesh1, ECoordinateSpace::World);
	}

	auto* camera{ Create<TCamera>() };
	camera->Use();

	auto* drawTimer{ Create<KTimer>() };
	drawTimer->SetDuration(1.0f / 120.0f);
	drawTimer->SetIsRepeat(true);
	drawTimer->GetEventCompleted().AddListener(KtDelegate(this, &SObjectManager::SubmitDrawObjects));
	drawTimer->Start();

	auto* logUPSTimer{ Create<KTimer>() };
	logUPSTimer->SetDuration(1.0f);
	logUPSTimer->SetIsRepeat(true);
	logUPSTimer->GetEventCompleted().AddListener(KtDelegate(this, &SObjectManager::LogUPS));
	logUPSTimer->Start();
}

void SObjectManager::Update()
{
	float updateTime{ 0.0f };
	updateTime += KtStopwatch::Time<float>(KtDelegate(this, &SObjectManager::InitObjects)); // todo: maybe put after delete
	updateTime += KtStopwatch::Time<float>(KtDelegate(this, &SObjectManager::UpdateObjects));
	updateTime += KtStopwatch::Time<float>(KtDelegate(this, &SObjectManager::DeleteObjects));

	updateTimeIndex_ = (updateTimeIndex_ + 1) % updateTimes_.size();
	updateTimesSum_ -= updateTimes_[updateTimeIndex_];
	updateTimes_[updateTimeIndex_] = updateTime;
	updateTimesSum_ += updateTimes_[updateTimeIndex_];

	if (canDraw_)
	{
		canDraw_ = false;

		float drawTime{ KtStopwatch::Time<float>(KtDelegate(&Framework.GetRenderer(), &KtRenderer::DrawFrame)) };

		drawTimeIndex_ = (drawTimeIndex_ + 1) % drawTimes_.size();
		drawTimesSum_ -= drawTimes_[drawTimeIndex_];
		drawTimes_[drawTimeIndex_] = drawTime;
		drawTimesSum_ += drawTimes_[drawTimeIndex_];
	}
}

void SObjectManager::Cleanup()
{
	KtPool<KObject*> objects{};
	objects.Merge(inits_);
	objects.Merge(objects_);
	for (auto* object : objects)
	{
		object->Cleanup();
	}
	for (auto* object : objects)
	{
		KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "DEL object %s", object->GetName().c_str());
		delete object;
	}
	deletes_.Clear();
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

	for (size_t i{ 0 }; i < inits_.Size(); ++i)
	{
		auto* object{ inits_[i] };
		object->Init();
		objects_.Add(object);
		object->objectIndex_ = objects_.LastIndex();
	}
	inits_.Clear();

	KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "object count %llu", objects_.Size());
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

	for (size_t i{ 0 }; i < deletes_.Size(); ++i)
	{
		auto* object{ deletes_[i] };

		object->Cleanup();

		const size_t index{ object->objectIndex_ };
		const auto removeResult{ objects_.RemoveAt(index) };
		if (removeResult == KtPoolRemoveResult::ItemSwappedAndRemoved)
		{
			objects_[index]->objectIndex_ = index;
		}
	}
	for (auto* object : deletes_)
	{
		KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_OBJECT, "DEL object %s", object->GetName().c_str());
		typeRegistry_[typeid(*object)].erase(object);
		delete object;
	}

	deletes_.Clear();
}

void SObjectManager::SubmitDrawObjects()
{
	canDraw_ = true;
}

float SObjectManager::GetAverageUpdateTime() const
{
	return updateTimesSum_ / updateTimes_.size();
}

float SObjectManager::GetAverageDrawTime() const
{
	return drawTimesSum_ / drawTimes_.size();
}

void SObjectManager::LogUPS() const
{
	KT_LOG_KE(KT_LOG_IMPORTANCE_LEVEL_HIGH, "%.2f ups", 1.0f / GetAverageUpdateTime());
}
