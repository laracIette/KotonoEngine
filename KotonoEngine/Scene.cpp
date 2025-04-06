#include "Scene.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include "Engine.h"
#include "ObjectManager.h"
#include "Mesh.h"

void OScene::Init()
{
	Base::Init();
	_viewport = &WindowViewport;
}

void OScene::Load()
{
	Deserialize();
	for (auto* sceneObject : _sceneObjects)
	{
		sceneObject->SetViewport(_viewport);
	}
}

void OScene::Unload()
{
	for (auto* sceneObject : _sceneObjects)
	{
		sceneObject->SetIsDelete(true);
	}
	_sceneObjects.clear();
}

void OScene::Reload()
{
	Unload();
	Load();
}

void OScene::Add(TSceneObject* sceneObject)
{
	_sceneObjects.insert(sceneObject);
}

void OScene::Remove(TSceneObject* sceneObject)
{
	_sceneObjects.erase(sceneObject);
}

void OScene::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	for (const auto* sceneObject : _sceneObjects)
	{
		nlohmann::json jsonSceneObject;
		sceneObject->SerializeTo(jsonSceneObject);
		json["sceneObjects"].push_back(jsonSceneObject);
	}
}

void OScene::DeserializeFrom(const nlohmann::json& json)
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

TSceneObject* OScene::GetSceneObject(const std::string_view type)
{
	if (type == "TSceneObject")    return Engine.GetObjectManager().Create<TSceneObject>();
	else if (type == "TMesh")      return Engine.GetObjectManager().Create<TMesh>();
	return nullptr;
}
