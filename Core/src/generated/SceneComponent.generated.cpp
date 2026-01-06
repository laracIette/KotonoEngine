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

std::vector<UVariableInfo> KSceneComponent::GetMemberVariables() const
{
	auto result{ Base::GetMemberVariables() };
	result.insert(result.end(), {
		{ "KtPool<UPtr<KSceneComponent>>", "children_", offsetof(Self, children_) },
		{ "UTransform", "transform_", offsetof(Self, transform_) },
		{ "EVisibility", "visibility_", offsetof(Self, visibility_) },
		{ "EMobility", "mobility_", offsetof(Self, mobility_) },
		{ "size", "componentIndex_", offsetof(Self, componentIndex_) },
		{ "size", "childrenIndex_", offsetof(Self, childrenIndex_) },

	});
	return result;
}

UPtr<KSceneComponent> KSceneComponent::Ptr() const
{
	return static_cast<UPtrOwner<KSceneComponent>*>(ptrOwner_);
}
