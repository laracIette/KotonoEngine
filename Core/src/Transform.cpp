#include "Transform.h"
#include "serialize_glm.h"

void USerialize<UTransform>::operator()(nlohmann::json& json, const UTransform& v) const
{
	USerialize<glm::vec3>{}(get(json, "position"), v.position);
	USerialize<glm::quat>{}(get(json, "rotation"), v.rotation);
	USerialize<glm::vec3>{}(get(json, "scale"), v.scale);
}

void UDeserialize<UTransform>::operator()(const nlohmann::json& json, UTransform& v) const
{
	UDeserialize<glm::vec3>{}(get(json, "position"), v.position);
	UDeserialize<glm::quat>{}(get(json, "rotation"), v.rotation);
	UDeserialize<glm::vec3>{}(get(json, "scale"), v.scale);
}
