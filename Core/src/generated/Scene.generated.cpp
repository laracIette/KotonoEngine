#include "Scene.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "SceneObject.h"

void KScene::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	serialize(json["sceneObjects_"], sceneObjects_);
}

void KScene::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	deserialize(json.at("sceneObjects_"), sceneObjects_);
}

UPtr<KScene> KScene::Ptr() const
{
	return static_cast<UPtrOwner<KScene>*>(ptrOwner_);
}
