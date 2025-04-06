#include "SceneObject.h"
#include <nlohmann/json.hpp>

void TSceneObject::Init()
{
	Base::Init();
	_visibility = VISIBILITY_EDITOR_AND_GAME;
	_viewport = &WindowViewport;
}

const UTransform& TSceneObject::GetTransform() const
{ 
	return _transform; 
}

UTransform& TSceneObject::GetTransform()
{
	return _transform;
}

const EVisibility TSceneObject::GetVisibility() const
{
	return _visibility;
}

KtViewport* TSceneObject::GetViewport() const
{
	return _viewport;
}

TSceneObject* TSceneObject::GetParent() const
{
	return _parent;
}

void TSceneObject::SetVisibility(const EVisibility visibility)
{
	_visibility = visibility;
}

void TSceneObject::SetViewport(KtViewport* viewport)
{
	_viewport = viewport;
}

void TSceneObject::SetParent(TSceneObject* sceneObject)
{
	_parent = sceneObject;
	_transform.SetParent(_parent ? &_parent->_transform : nullptr);
}

void TSceneObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	json["parent"] = _parent ? static_cast<std::string>(_parent->GetGuid()) : "";
	json["transform"]["position"]["x"] = _transform.GetRelativePosition().x;
	json["transform"]["position"]["y"] = _transform.GetRelativePosition().y;
	json["transform"]["position"]["z"] = _transform.GetRelativePosition().z;
	json["transform"]["rotation"]["w"] = _transform.GetRelativeRotation().w;
	json["transform"]["rotation"]["x"] = _transform.GetRelativeRotation().x;
	json["transform"]["rotation"]["y"] = _transform.GetRelativeRotation().y;
	json["transform"]["rotation"]["z"] = _transform.GetRelativeRotation().z;
	json["transform"]["scale"]["x"] = _transform.GetRelativeScale().x;
	json["transform"]["scale"]["y"] = _transform.GetRelativeScale().y;
	json["transform"]["scale"]["z"] = _transform.GetRelativeScale().z;
}

void TSceneObject::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	// parent
	_transform.SetRelativePosition({
		json["transform"]["position"]["x"],
		json["transform"]["position"]["y"],
		json["transform"]["position"]["z"]
	});
	_transform.SetRelativeRotation({
		json["transform"]["rotation"]["w"],
		json["transform"]["rotation"]["x"],
		json["transform"]["rotation"]["y"],
		json["transform"]["rotation"]["z"]
	});
	_transform.SetRelativeScale({ 
		json["transform"]["scale"]["x"],
		json["transform"]["scale"]["y"],
		json["transform"]["scale"]["z"] 
	});
}
