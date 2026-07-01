#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <kotono_io/serialize_base.h>
struct UTransform final
{
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::quat rotation{ glm::identity<glm::quat>() };
    glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
};

template<>
struct USerialize<UTransform>
{
	void operator()(nlohmann::json& json, const UTransform& v) const;
};

template<>
struct UDeserialize<UTransform>
{
    void operator()(const nlohmann::json& json, UTransform& v) const;
};
