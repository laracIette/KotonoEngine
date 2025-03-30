#include <nlohmann/json.hpp>
#include "Scene.h"
#include "Engine.h"

#include "Mesh.h"

void OScene::Init()
{
	Base::Init();
	_viewport = &WindowViewport;
}

void OScene::Load()
{
	Base::Deserialize();
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
	SetIsDelete(true);
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
		
		TSceneObject* sceneObject = nullptr; 
		if (jsonSceneObject["type"] == "TSceneObject") sceneObject = Engine.GetObjectManager().Create<TSceneObject>();
		else if (jsonSceneObject["type"] == "TMesh")   sceneObject = Engine.GetObjectManager().Create<TMesh>();
		sceneObject->DeserializeFrom(jsonSceneObject);
		Add(sceneObject);
	}
}
