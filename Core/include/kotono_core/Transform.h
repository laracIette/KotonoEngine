#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "serialize_glm.h"
struct UTransform final
{
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::quat rotation{ glm::identity<glm::quat>() };
    glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
};

template <>
struct USerialize<UTransform>
{
	void operator()(nlohmann::json& json, const UTransform& v) const
	{
		USerialize<glm::vec3>{}(get(json, "position"), v.position);
		USerialize<glm::quat>{}(get(json, "rotation"), v.rotation);
		USerialize<glm::vec3>{}(get(json, "scale"), v.scale);
	}
};

template <>
struct UDeserialize<UTransform>
{
	void operator()(const nlohmann::json& json, UTransform& v) const
	{
		UDeserialize<glm::vec3>{}(get(json, "position"), v.position);
		UDeserialize<glm::quat>{}(get(json, "rotation"), v.rotation);
		UDeserialize<glm::vec3>{}(get(json, "scale"), v.scale);
	}
};
