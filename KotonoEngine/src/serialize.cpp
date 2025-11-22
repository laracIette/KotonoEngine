#include "serialize.h"
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <kotono_framework/Color.h>
#include <kotono_framework/Framework.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ImageTexture.h>
#include <kotono_framework/ImageTextureManager.h>
#include <kotono_framework/Model.h>
#include <kotono_framework/ModelManager.h>
#include <filesystem>
#include "Ptr.h"
#include "Object.h"
#include "Transform.h"
#include "Rect.h"
#include "Duration.h"

void serialize(nlohmann::json& json, const bool v)
{
    json = v;
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

void serialize(nlohmann::json& json, const float v)
{
    json = v;
}

void serialize(nlohmann::json& json, const double v)
{
    json = v;
}

void serialize(nlohmann::json& json, const EVisibility v)
{
    json = static_cast<char>(v);
}

void serialize(nlohmann::json& json, const EMobility v)
{
    json = static_cast<char>(v);
}

void serialize(nlohmann::json& json, const std::string& v)
{
    json = v;
}

void serialize(nlohmann::json& json, const std::filesystem::path& v)
{
    json = v.string();
}

void serialize(nlohmann::json& json, const glm::vec2& v)
{
    json["x"] = v.x;
    json["y"] = v.y;
}

void serialize(nlohmann::json& json, const glm::vec3& v)
{
    json["x"] = v.x;
    json["y"] = v.y;
    json["z"] = v.z;
}

void serialize(nlohmann::json& json, const glm::uvec2& v)
{
    json["x"] = v.x;
    json["y"] = v.y;
}

void serialize(nlohmann::json& json, const glm::ivec2& v)
{
    json["x"] = v.x;
    json["y"] = v.y;
}

void serialize(nlohmann::json& json, const glm::quat& v)
{
    json["w"] = v.w;
    json["x"] = v.x;
    json["y"] = v.y;
    json["z"] = v.z;
}

void serialize(nlohmann::json& json, const KtColor& v)
{
    json["r"] = v.r;
    json["g"] = v.g;
    json["b"] = v.b;
    json["a"] = v.a;
}

void serialize(nlohmann::json& json, const UGuid& v)
{
    json = static_cast<std::string>(v);
}

void serialize(nlohmann::json& json, const UTransform& v)
{
    serialize(json["position"], v.position);
    serialize(json["rotation"], v.rotation);
    serialize(json["scale"], v.scale);
}

void serialize(nlohmann::json& json, const URect& v)
{
    serialize(json["size"], v.size);
    serialize(json["position"], v.position);
    serialize(json["scale"], v.scale);
    json["rotation"] = v.rotation;
    //json["anchor"] = rect.anchor;
}

void serialize(nlohmann::json& json, const UDuration& v)
{
    serialize(json["isSeconds"], v.IsSeconds());
    if (v.IsSeconds())
    {
        serialize(json["duration"], v.Seconds());
    }
    else
    {
        serialize(json["duration"], v.Updates());
    }
}

void serialize(nlohmann::json& json, const KtShader* v)
{
    if (!v)
    {
        return;
    }
    serialize(json, v->Path());
}

void serialize(nlohmann::json& json, const KtImageTexture* v)
{
    if (!v)
    {
        return;
    }
    serialize(json, v->Path());
}

void serialize(nlohmann::json& json, const KtModel* v)
{
    if (!v)
    {
        return;
    } 
    serialize(json, v->Path());
}

void serialize_kobject(nlohmann::json& json, const KObject* v)
{
    if (!v)
    {
        return;
    }

    serialize(json, v->guid_);
    v->Serialize();
    //v->SerializeTo(json);
}

void deserialize(const nlohmann::json& json, bool& v)
{
    v = json;
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

void deserialize(const nlohmann::json& json, float& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, double& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, EVisibility& v)
{
    v = static_cast<EVisibility>(json.get<char>());
}

void deserialize(const nlohmann::json& json, EMobility& v)
{
    v = static_cast<EMobility>(json.get<char>());
}

void deserialize(const nlohmann::json& json, std::string& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, std::filesystem::path& v)
{
    v = json.get<std::string>();
}

void deserialize(const nlohmann::json& json, glm::vec2& v)
{
    v.x = json.at("x");
    v.y = json.at("y");
}

void deserialize(const nlohmann::json& json, glm::vec3& v)
{
    v.x = json.at("x");
    v.y = json.at("y");
    v.z = json.at("z");
}

void deserialize(const nlohmann::json& json, glm::uvec2& v)
{
    v.x = json.at("x");
    v.y = json.at("y");
}

void deserialize(const nlohmann::json& json, glm::ivec2& v)
{
    v.x = json.at("x");
    v.y = json.at("y");
}

void deserialize(const nlohmann::json& json, glm::quat& v)
{
    v.w = json.at("w");
    v.x = json.at("x");
    v.y = json.at("y");
    v.z = json.at("z");
}

void deserialize(const nlohmann::json& json, KtColor& v)
{
    v.r = json.at("r");
    v.g = json.at("g");
    v.b = json.at("b");
    v.a = json.at("a");
}

void deserialize(const nlohmann::json& json, UGuid& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, UTransform& v)
{
    deserialize(json.at("position"), v.position);
    deserialize(json.at("rotation"), v.rotation);
    deserialize(json.at("scale"), v.scale);
}

void deserialize(const nlohmann::json& json, URect& v)
{
    deserialize(json.at("size"), v.size);
    deserialize(json.at("position"), v.position);
    deserialize(json.at("scale"), v.scale);
    v.rotation = json.at("rotation");
    //rect.anchor = json.at("anchor");
}

void deserialize(const nlohmann::json& json, UDuration& v)
{
    if (json.at("isSeconds").get<bool>())
    {
        v = UDuration::FromSeconds(json.at("duration").get<float>());
    }
    else
    {
        v = UDuration::FromUpdates(json.at("duration").get<uint64_t>());
    }
}

void deserialize(const nlohmann::json& json, KtShader*& v)
{
    const std::filesystem::path path(json.get<std::string>());
    v = Framework.ShaderManager().Get(path);
}

void deserialize(const nlohmann::json& json, KtImageTexture*& v)
{
    const std::filesystem::path path(json.get<std::string>());
    v = Framework.ImageTextureManager().Get(path);
}

void deserialize(const nlohmann::json& json, KtModel*& v)
{
    const std::filesystem::path path(json.get<std::string>());
    v = Framework.ModelManager().Get(path);
}

void deserialize_kobject(const nlohmann::json& json, KObject* v)
{
    if (!v)
    {
        return;
    }

    deserialize(json, v->guid_);
    v->Deserialize();
    //v->DeserializeFrom(json);
}
