#pragma once
#include <nlohmann/json_fwd.hpp>
#include <glm/fwd.hpp>
#include "Ptr.h"

#define SERIALIZE

namespace std
{
	namespace filesystem
	{
		class path;
	}
}

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
void serialize(nlohmann::json& json, const float v);
void serialize(nlohmann::json& json, const double v);

void serialize(nlohmann::json& json, const std::string& v);
void serialize(nlohmann::json& json, const std::filesystem::path& v);

void serialize(nlohmann::json& json, const glm::vec2& v);
void serialize(nlohmann::json& json, const glm::vec3& v);
void serialize(nlohmann::json& json, const glm::uvec2& v);
void serialize(nlohmann::json& json, const glm::ivec2& v);
void serialize(nlohmann::json& json, const glm::quat& v);

void serialize(nlohmann::json& json, const KtColor& v);

void serialize(nlohmann::json& json, const UGuid& v);
void serialize(nlohmann::json& json, const UTransform& v);
void serialize(nlohmann::json& json, const URect& v);

template <class T>
	requires std::is_base_of_v<KObject, T>
void serialize(nlohmann::json& json, const UPtr<T>& v)
{
	serialize_kobject(json, static_cast<KObject*>(v.Get()));
}
void serialize_kobject(nlohmann::json& json, const KObject* object);


void deserialize(const nlohmann::json& json, int8_t& v);
void deserialize(const nlohmann::json& json, int16_t& v);
void deserialize(const nlohmann::json& json, int32_t& v);
void deserialize(const nlohmann::json& json, int64_t& v);
void deserialize(const nlohmann::json& json, uint8_t& v);
void deserialize(const nlohmann::json& json, uint16_t& v);
void deserialize(const nlohmann::json& json, uint32_t& v);
void deserialize(const nlohmann::json& json, uint64_t& v);
void deserialize(const nlohmann::json& json, float& v);
void deserialize(const nlohmann::json& json, double& v);

void deserialize(const nlohmann::json& json, std::string& v);
void deserialize(const nlohmann::json& json, std::filesystem::path& v);

void deserialize(const nlohmann::json& json, glm::vec2& v);
void deserialize(const nlohmann::json& json, glm::vec3& v);
void deserialize(const nlohmann::json& json, glm::uvec2& v);
void deserialize(const nlohmann::json& json, glm::ivec2& v);
void deserialize(const nlohmann::json& json, glm::quat& v);

void deserialize(const nlohmann::json& json, KtColor& v);

void deserialize(const nlohmann::json& json, UGuid& v);
void deserialize(const nlohmann::json& json, UTransform& v);
void deserialize(const nlohmann::json& json, URect& v);

template <class T>
	requires std::is_base_of_v<KObject, T>
void deserialize(const nlohmann::json& json, UPtr<T>& v)
{
	deserialize_kobject(json, static_cast<KObject*>(v.Get()));
}
void deserialize_kobject(const nlohmann::json& json, KObject* v);

