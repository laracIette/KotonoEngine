#pragma once
#include "Ptr.h"
#include "Visibility.h"
#include <glm/fwd.hpp>
#include <kotono_graphics/Mobility.h>
#include <nlohmann/json_fwd.hpp>

#define SERIALIZE

namespace std
{
	namespace filesystem
	{
		class path;
	}
}

struct KtColor;
struct UGuid;
struct UTransform;
struct URect;
class KtShader;
class KtImageTexture;
class KtModel;
class KObject;

void serialize(nlohmann::json& json, const bool v);
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

void serialize(nlohmann::json& json, const EVisibility v);
void serialize(nlohmann::json& json, const EMobility v);

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

void serialize(nlohmann::json& json, const KtShader* v);
void serialize(nlohmann::json& json, const KtImageTexture* v);
void serialize(nlohmann::json& json, const KtModel* v);

void serialize_kobject(nlohmann::json& json, const UPtr<KObject>& v);
template <std::derived_from<KObject> T>
void serialize(nlohmann::json& json, const UPtr<T>& v)
{
	serialize_kobject(json, v);
}


void deserialize(const nlohmann::json& json, bool& v);
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

void deserialize(const nlohmann::json& json, EVisibility& v);
void deserialize(const nlohmann::json& json, EMobility& v);

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

void deserialize(const nlohmann::json& json, KtShader*& v);
void deserialize(const nlohmann::json& json, KtImageTexture*& v);
void deserialize(const nlohmann::json& json, KtModel*& v);

UPtr<KObject> deserialize_kobject(const nlohmann::json& json);
template <std::derived_from<KObject> T>
void deserialize(const nlohmann::json& json, UPtr<T>& v)
{
	v = deserialize_kobject(json);
}
