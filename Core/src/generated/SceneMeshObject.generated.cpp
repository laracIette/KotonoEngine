#include "SceneMeshObject.h"
#include "Ptr.h"
#include "serialize.h"
#include <nlohmann/json.hpp>

void TSceneMeshObject::SerializeTo(nlohmann::json& json) const
{
	Base::SerializeTo(json);
}

void TSceneMeshObject::DeserializeFrom(const nlohmann::json& json)
{
	Base::DeserializeFrom(json);
}

UPtr<TSceneMeshObject> TSceneMeshObject::Ptr() const
{
	return static_cast<UPtrOwner<TSceneMeshObject>*>(ptrOwner_);
}
