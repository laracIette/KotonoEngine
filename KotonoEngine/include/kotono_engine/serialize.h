#pragma once
#include <nlohmann/json_fwd.hpp>
#include <glm/fwd.hpp>
#include "Ptr.h"

#define SERIALIZE

struct KtColor;
class UGuid;
struct UTransform;
struct URect;
class KObject;

void serialize(nlohmann::json& json, const int8_t v);
void serialize(nlohmann::json& json, const int16_t v);
void serialize(nlohmann::json& json, const int32_t v);
void serialize(nlohmann::json& json, const int64_t v);
void serialize(nlohmann::json& json, const uint8_t v);
void serialize(nlohmann::json& json, const uint16_t v);
void serialize(nlohmann::json& json, const uint32_t v);
void serialize(nlohmann::json& json, const uint64_t v);

void serialize(nlohmann::json& json, const glm::vec2& vec);
void serialize(nlohmann::json& json, const glm::vec3& vec);
void serialize(nlohmann::json& json, const glm::uvec2& vec);
void serialize(nlohmann::json& json, const glm::ivec2& vec);
void serialize(nlohmann::json& json, const glm::quat& quat);

void serialize(nlohmann::json& json, const KtColor& color);

void serialize(nlohmann::json& json, const UGuid& guid);
void serialize(nlohmann::json& json, const UTransform& transform);
void serialize(nlohmann::json& json, const URect& rect);

template <class T>
	requires std::is_base_of_v<KObject, T>
void serialize(nlohmann::json& json, const UPtr<T>& object)
{
	serialize(json, UPtr<KObject>(object));
}
void serialize(nlohmann::json& json, const UPtr<KObject>& object);

void deserialize(const nlohmann::json& json, glm::vec2& vec);
void deserialize(const nlohmann::json& json, glm::vec3& vec);
void deserialize(const nlohmann::json& json, glm::uvec2& vec);
void deserialize(const nlohmann::json& json, glm::ivec2& vec);
void deserialize(const nlohmann::json& json, glm::quat& quat);

void deserialize(const nlohmann::json& json, KtColor& color);

void deserialize(const nlohmann::json& json, UGuid& guid);
void deserialize(const nlohmann::json& json, UTransform& transform);
void deserialize(const nlohmann::json& json, URect& rect);

void deserialize(const nlohmann::json& json, UPtr<KObject>& object);

