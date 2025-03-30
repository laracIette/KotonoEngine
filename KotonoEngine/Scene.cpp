#include <nlohmann/json.hpp>
#include "Scene.h"
#include "Engine.h"

void OScene::Init()
{
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

void OScene::Serialize(nlohmann::json& json) const
{
	Base::Serialize(json);
	for (const auto* sceneObject : _sceneObjects)
	{
		json["sceneObjects"].push_back(sceneObject->GetGuid());
	}
}

void OScene::Deserialize(const nlohmann::json& json)
{
	Base::Deserialize(json);
	for (const auto& sceneObjectPath : json["sceneObjects"])
	{
		auto* sceneObject = Engine.GetObjectManager().Create<TSceneObject>();
		sceneObject->SetPath(sceneObjectPath);
		sceneObject->Deserialize();
		_sceneObjects.push_back(sceneObject);
	}
}
