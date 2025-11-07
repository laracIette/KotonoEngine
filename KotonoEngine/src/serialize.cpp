#include "serialize.h"
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <kotono_framework/Color.h>
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

void deserialize(const nlohmann::json& json, glm::vec2& vec)
{
    vec.x = json["x"];
    vec.y = json["y"];
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
