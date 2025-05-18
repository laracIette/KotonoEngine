#include "Scene.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <kotono_framework/Viewport.h>
#include "Engine.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "SceneMeshComponent.h"
#include "SceneMeshObject.h"

void KScene::Init()
{
	Base::Init();

	viewport_ = &WindowViewport;
}

void KScene::Load()
{
	Deserialize();
	for (auto* sceneObject : _sceneObjects)
	{
		sceneObject->SetViewport(viewport_);
	}
}

void KScene::Unload()
{
	for (auto* sceneObject : _sceneObjects)
	{
		sceneObject->SetIsDelete(true);
	}
	_sceneObjects.clear();
}

void KScene::Reload()
{
	Unload();
	Load();
}

void KScene::Add(TSceneObject* sceneObject)
{
	_sceneObjects.insert(sceneObject);
}

void KScene::Remove(TSceneObject* sceneObject)
{
	_sceneObjects.erase(sceneObject);
}

void KScene::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	for (const auto* sceneObject : _sceneObjects)
	{
		nlohmann::json jsonSceneObject;
		sceneObject->SerializeTo(jsonSceneObject);
		json["sceneObjects"].push_back(jsonSceneObject);
	}
}

void KScene::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	for (const auto& jsonSceneObject : json["sceneObjects"])
	{
		if (TSceneObject* sceneObject = GetSceneObject(jsonSceneObject["type"]))
		{
			sceneObject->DeserializeFrom(jsonSceneObject);
			Add(sceneObject);
		}
		else
		{
			std::cerr << "Type of not supported by scene deserialization" << std::endl;
		}
	}
}

TSceneObject* KScene::GetSceneObject(const std::string_view type)
{
	if (type == "TSceneObject")             return Engine.GetObjectManager().Create<TSceneObject>();
	else if (type == "TSceneMeshObject")    return Engine.GetObjectManager().Create<TSceneMeshObject>();
	return nullptr;
}
