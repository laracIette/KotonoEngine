#include "SceneComponent.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>
#include "SceneObject.h"
#include "SceneObject.h"

void KSceneComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	serialize(json["children_"], children_);
	serialize(json["transform_"], transform_);
	serialize(json["visibility_"], visibility_);
	serialize(json["mobility_"], mobility_);
	serialize(json["componentIndex_"], componentIndex_);
	serialize(json["childrenIndex_"], childrenIndex_);
}

void KSceneComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	deserialize(json.at("children_"), children_);
	deserialize(json.at("transform_"), transform_);
	deserialize(json.at("visibility_"), visibility_);
	deserialize(json.at("mobility_"), mobility_);
	deserialize(json.at("componentIndex_"), componentIndex_);
	deserialize(json.at("childrenIndex_"), childrenIndex_);
}

UPtr<KSceneComponent> KSceneComponent::Ptr() const
{
	return static_cast<UPtrOwner<KSceneComponent>*>(ptrOwner_);
}
