#include "SceneMeshComponent.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>


void KSceneMeshComponent::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
	serialize(json["shader_"], shader_);
	serialize(json["model_"], model_);

}

void KSceneMeshComponent::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
	deserialize(json.at("shader_"), shader_);
	deserialize(json.at("model_"), model_);

}

std::vector<UVariableInfo> KSceneMeshComponent::GetMemberVariables() const
{
	auto result{ Base::GetMemberVariables() };
	result.insert(result.end(), {
		{ "KtShader*", offsetof(Self, shader_) },
		{ "KtModel*", offsetof(Self, model_) },

	});
	return result;
}

UPtr<KSceneMeshComponent> KSceneMeshComponent::Ptr() const
{
	return static_cast<UPtrOwner<KSceneMeshComponent>*>(ptrOwner_);
}
