#include "serialize.h"
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <kotono_framework/Color.h>
#include <filesystem>
#include "Ptr.h"
#include "Object.h"
#include "Transform.h"
#include "Rect.h"

void serialize(nlohmann::json& json, const glm::vec2& vec)
{
    json["x"] = vec.x;
    json["y"] = vec.y;
}

void serialize(nlohmann::json& json, const glm::vec3& vec)
{
    json["x"] = vec.x;
    json["y"] = vec.y;
    json["z"] = vec.z;
}

void serialize(nlohmann::json& json, const glm::uvec2& vec)
{
    json["x"] = vec.x;
    json["y"] = vec.y;
}

void serialize(nlohmann::json& json, const glm::ivec2& vec)
{
    json["x"] = vec.x;
    json["y"] = vec.y;
}

void serialize(nlohmann::json& json, const glm::quat& quat)
{
    json["w"] = quat.w;
    json["x"] = quat.x;
    json["y"] = quat.y;
    json["z"] = quat.z;
}

void serialize(nlohmann::json& json, const KtColor& color)
{
    json["r"] = color.r;
    json["g"] = color.g;
    json["b"] = color.b;
    json["a"] = color.a;
}

void serialize(nlohmann::json& json, const UGuid& guid)
{
    json = static_cast<std::string>(guid);
}

void serialize(nlohmann::json& json, const UTransform& transform)
{
    serialize(json["position"], transform.position);
    serialize(json["rotation"], transform.rotation);
    serialize(json["scale"], transform.scale);
}

void serialize(nlohmann::json& json, const URect& rect)
{
    serialize(json["size"], rect.size);
    serialize(json["position"], rect.position);
    serialize(json["scale"], rect.scale);
    json["rotation"] = rect.rotation;
    //json["anchor"] = rect.anchor;
}

void serialize(nlohmann::json& json, const UPtr<KObject>& object)
{
    object->SerializeTo(json);
}

void deserialize(const nlohmann::json& json, int8_t& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, int16_t& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, int32_t& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, int64_t& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, uint8_t& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, uint16_t& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, uint32_t& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, uint64_t& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, std::string& string)
{
    string = json;
}

void deserialize(const nlohmann::json& json, std::filesystem::path& path)
{
    path = json.get<std::string>();
}

void deserialize(const nlohmann::json& json, glm::vec2& vec)
{
    vec.x = json["x"];
    vec.y = json["y"];
}

void deserialize(const nlohmann::json& json, glm::vec3& vec)
{
    vec.x = json["x"];
    vec.y = json["y"];
    vec.z = json["z"];
}

void deserialize(const nlohmann::json& json, glm::uvec2& vec)
{
    vec.x = json["x"];
    vec.y = json["y"];
}

void deserialize(const nlohmann::json& json, glm::ivec2& vec)
{
    vec.x = json["x"];
    vec.y = json["y"];
}

void deserialize(const nlohmann::json& json, glm::quat& quat)
{
    quat.w = json["w"];
    quat.x = json["x"];
    quat.y = json["y"];
    quat.z = json["z"];
}

void deserialize(const nlohmann::json& json, KtColor& color)
{
    color.r = json["r"];
    color.g = json["g"];
    color.b = json["b"];
    color.a = json["a"];
}

void deserialize(const nlohmann::json& json, UGuid& guid)
{
    guid = json;
}

void deserialize(const nlohmann::json& json, UTransform& transform)
{
    deserialize(json["position"], transform.position);
    deserialize(json["rotation"], transform.rotation);
    deserialize(json["scale"], transform.scale);
}

void deserialize(const nlohmann::json& json, URect& rect)
{
    deserialize(json["size"], rect.size);
    deserialize(json["position"], rect.position);
    deserialize(json["scale"], rect.scale);
    rect.rotation = json["rotation"];
    //rect.anchor = json["anchor"];
}

void deserialize(const nlohmann::json& json, UPtr<KObject>& object)
{
    object->DeserializeFrom(json);
}

void serialize(nlohmann::json& json, const int8_t v)
{
    json = v;
}

void serialize(nlohmann::json& json, const int16_t v)
{
    json = v;
}

void serialize(nlohmann::json& json, const int32_t v)
{
    json = v;
}

void serialize(nlohmann::json& json, const int64_t v)
{
    json = v;
}

void serialize(nlohmann::json& json, const uint8_t v)
{
    json = v;
}

void serialize(nlohmann::json& json, const uint16_t v)
{
    json = v;
}

void serialize(nlohmann::json& json, const uint32_t v)
{
    json = v;
}

void serialize(nlohmann::json& json, const uint64_t v)
{
    json = v;
}

void serialize(nlohmann::json& json, const std::string& string)
{
    json = string;
}

void serialize(nlohmann::json& json, const std::filesystem::path& path)
{
    json = path;
}
