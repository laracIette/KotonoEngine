#pragma once
#include "Ptr.h"
#include "Visibility.h"
#include <glm/fwd.hpp>
#include <kotono_graphics/Mobility.h>
#include <nlohmann/json_fwd.hpp>

#define SERIALIZE

struct UColor;
class UGuid;
struct UTransform;
struct URect;
class KtShader;
class KtTexture;
class KtModel;
class KObject;
class UPath;

void serialize(nlohmann::json& json, const bool v);
void serialize(nlohmann::json& json, const i8 v);
void serialize(nlohmann::json& json, const i16 v);
void serialize(nlohmann::json& json, const i32 v);
void serialize(nlohmann::json& json, const i64 v);
void serialize(nlohmann::json& json, const u8 v);
void serialize(nlohmann::json& json, const u16 v);
void serialize(nlohmann::json& json, const u32 v);
void serialize(nlohmann::json& json, const u64 v);
void serialize(nlohmann::json& json, const float v);
void serialize(nlohmann::json& json, const double v);

void serialize(nlohmann::json& json, const EVisibility v);
void serialize(nlohmann::json& json, const EMobility v);

void serialize(nlohmann::json& json, const std::string& v);
void serialize(nlohmann::json& json, const UPath& v);

void serialize(nlohmann::json& json, const glm::vec2& v);
void serialize(nlohmann::json& json, const glm::vec3& v);
void serialize(nlohmann::json& json, const glm::uvec2& v);
void serialize(nlohmann::json& json, const glm::ivec2& v);
void serialize(nlohmann::json& json, const glm::quat& v);

void serialize(nlohmann::json& json, const UColor& v);

void serialize(nlohmann::json& json, const UGuid& v);
void serialize(nlohmann::json& json, const UTransform& v);
void serialize(nlohmann::json& json, const URect& v);

void serialize(nlohmann::json& json, const KtShader* v);
void serialize(nlohmann::json& json, const KtTexture* v);
void serialize(nlohmann::json& json, const KtModel* v);

void serialize_kobject(nlohmann::json& json, const UPtr<KObject>& v);
template <std::derived_from<KObject> T>
void serialize(nlohmann::json& json, const UPtr<T>& v)
{
	serialize_kobject(json, v);
}


void deserialize(const nlohmann::json& json, bool& v);
void deserialize(const nlohmann::json& json, i8& v);
void deserialize(const nlohmann::json& json, i16& v);
void deserialize(const nlohmann::json& json, i32& v);
void deserialize(const nlohmann::json& json, i64& v);
void deserialize(const nlohmann::json& json, u8& v);
void deserialize(const nlohmann::json& json, u16& v);
void deserialize(const nlohmann::json& json, u32& v);
void deserialize(const nlohmann::json& json, u64& v);
void deserialize(const nlohmann::json& json, float& v);
void deserialize(const nlohmann::json& json, double& v);

void deserialize(const nlohmann::json& json, EVisibility& v);
void deserialize(const nlohmann::json& json, EMobility& v);

void deserialize(const nlohmann::json& json, std::string& v);
void deserialize(const nlohmann::json& json, UPath& v);

void deserialize(const nlohmann::json& json, glm::vec2& v);
void deserialize(const nlohmann::json& json, glm::vec3& v);
void deserialize(const nlohmann::json& json, glm::uvec2& v);
void deserialize(const nlohmann::json& json, glm::ivec2& v);
void deserialize(const nlohmann::json& json, glm::quat& v);

void deserialize(const nlohmann::json& json, UColor& v);

void deserialize(const nlohmann::json& json, UGuid& v);
void deserialize(const nlohmann::json& json, UTransform& v);
void deserialize(const nlohmann::json& json, URect& v);

void deserialize(const nlohmann::json& json, KtShader*& v);
void deserialize(const nlohmann::json& json, KtTexture*& v);
void deserialize(const nlohmann::json& json, KtModel*& v);

UPtr<KObject> deserialize_kobject(const nlohmann::json& json);
template <std::derived_from<KObject> T>
void deserialize(const nlohmann::json& json, UPtr<T>& v)
{
	v = TryCast<T>(deserialize_kobject(json));
}
