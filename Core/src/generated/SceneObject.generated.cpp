#include "SceneObject.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "SceneComponent.h"
#include "Scene.h"

void TSceneObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	serialize(json["rootComponent_"], rootComponent_);
	serialize(json["sceneComponents_"], sceneComponents_);
	serialize(json["children_"], children_);
	serialize(json["childrenIndex_"], childrenIndex_);
}

void TSceneObject::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	deserialize(json.at("rootComponent_"), rootComponent_);
	deserialize(json.at("sceneComponents_"), sceneComponents_);
	deserialize(json.at("children_"), children_);
	deserialize(json.at("childrenIndex_"), childrenIndex_);
}

UPtr<TSceneObject> TSceneObject::Ptr() const
{
	return static_cast<UPtrOwner<TSceneObject>*>(ptrOwner_);
}
