#include "serialize.h"
#include "Object.h"
#include "ObjectFactory.h"
#include "Rect.h"
#include "Transform.h"
#include <glm/glm.hpp>
#include <kotono_graphics/Color.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/ModelManager.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/Texture.h>
#include <kotono_graphics/TextureManager.h>
#include <nlohmann/json.hpp>

void serialize(nlohmann::json& json, const bool v)
{
    json = v;
}

void serialize(nlohmann::json& json, const i8 v)
{
    json = v;
}

void serialize(nlohmann::json& json, const i16 v)
{
    json = v;
}

void serialize(nlohmann::json& json, const i32 v)
{
    json = v;
}

void serialize(nlohmann::json& json, const i64 v)
{
    json = v;
}

void serialize(nlohmann::json& json, const u8 v)
{
    json = v;
}

void serialize(nlohmann::json& json, const u16 v)
{
    json = v;
}

void serialize(nlohmann::json& json, const u32 v)
{
    json = v;
}

void serialize(nlohmann::json& json, const u64 v)
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

void serialize(nlohmann::json& json, const UPath& v)
{
    json = v.ToString();
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

void serialize(nlohmann::json& json, const UColor& v)
{
    json["r"] = v.r;
    json["g"] = v.g;
    json["b"] = v.b;
    json["a"] = v.a;
}

void serialize(nlohmann::json& json, const UGuid& v)
{
    json = v.ToString();
}

void serialize(nlohmann::json& json, const UTransform& v)
{
    serialize(json["position"], v.position);
    serialize(json["rotation"], v.rotation);
    serialize(json["scale"], v.scale);
}

void serialize(nlohmann::json& json, const URect& v)
{
    //serialize(json["size"], v.size);
    serialize(json["position"], v.position);
    serialize(json["scale"], v.scale);
    json["rotation"] = v.rotation;
    json["layer"] = v.layer;
    //json["anchor"] = rect.anchor;
}

void serialize(nlohmann::json& json, const KtShader* v)
{
    if (!v)
    {
        return;
    }
    serialize(json, v->Path());
}

void serialize(nlohmann::json& json, const KtTexture* v)
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

void serialize_kobject(nlohmann::json& json, const UPtr<KObject>& v)
{
    if (v)
    {
        serialize(json, v->Guid());
        v->Serialize();
    }
    else
    {
        json = "";
    }
}

void make_array(nlohmann::json& json)
{
    json = nlohmann::json::array({});
}

nlohmann::json& get_next(nlohmann::json& json)
{
	return json.emplace_back();
}

void deserialize(const nlohmann::json& json, bool& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, i8& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, i16& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, i32& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, i64& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, u8& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, u16& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, u32& v)
{
    v = json;
}

void deserialize(const nlohmann::json& json, u64& v)
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

void deserialize(const nlohmann::json& json, UPath& v)
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

void deserialize(const nlohmann::json& json, UColor& v)
{
    v.r = json.at("r");
    v.g = json.at("g");
    v.b = json.at("b");
    v.a = json.at("a");
}

void deserialize(const nlohmann::json& json, UGuid& v)
{
    const auto string{ json.get<std::string>() };
    if (!string.empty())
    {
        v = json;
    }
}

void deserialize(const nlohmann::json& json, UTransform& v)
{
    deserialize(json.at("position"), v.position);
    deserialize(json.at("rotation"), v.rotation);
    deserialize(json.at("scale"), v.scale);
}

void deserialize(const nlohmann::json& json, URect& v)
{
    //deserialize(json.at("size"), v.size);
    deserialize(json.at("position"), v.position);
    deserialize(json.at("scale"), v.scale);
    v.rotation = json.at("rotation");
    v.layer = json.at("layer");
    //rect.anchor = json.at("anchor");
}

void deserialize(const nlohmann::json& json, KtShader*& v)
{
    const UPath path(json.get<std::string>());
    v = ShaderManager.Get(path);
}

void deserialize(const nlohmann::json& json, KtTexture*& v)
{
    const UPath path(json.get<std::string>());
    v = TextureManager.Get(path);
}

void deserialize(const nlohmann::json& json, KtModel*& v)
{
    const UPath path(json.get<std::string>());
    v = ModelManager.Get(path);
}

UPtr<KObject> deserialize_kobject(const nlohmann::json& json)
{
    UGuid guid{};
    deserialize(json, guid);
    return ObjectFactory.Get(guid);
}

size get_size(const nlohmann::json& json)
{
    return json.size();
}

const nlohmann::json& get_at(const nlohmann::json& json, size index)
{
    return json.at(index);
}
