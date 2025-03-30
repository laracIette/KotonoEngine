#include <nlohmann/json.hpp>
#include "Object.h"
#include <kotono_framework/Serializer.h>
#include <regex>

void OObject::Init()
{
}

void OObject::Update() 
{
}

void OObject::Cleanup() 
{
}

const UGuid& OObject::GetGuid() const
{
    return _guid;
}

const std::filesystem::path& OObject::GetPath() const
{
    return _path;
}

const std::string& OObject::GetName() const
{
    return _name;
}

const bool OObject::GetIsDelete() const
{
    return _isDelete;
}

void OObject::SetName(const std::string& name)
{
    _name = name;
}

void OObject::SetPath(const std::filesystem::path& path)
{
    _path = path;
}

void OObject::SetIsDelete(const bool isDelete)
{
    _isDelete = isDelete;
}

void OObject::Serialize() const
{
    nlohmann::json json;
    KtSerializer serializer;
    Serialize(json);
    serializer.WriteData(_path, json);
}

void OObject::Deserialize()
{
    nlohmann::json json;
    KtSerializer serializer;
    serializer.ReadData(_path, json);
    Deserialize(json);
}

void OObject::Serialize(nlohmann::json& json) const
{
    json["guid"] = _guid;
    json["type"] = std::regex_replace(typeid(*this).name(), std::regex(R"(^(class |struct ))"), "");
    json["name"] = _name;
}

void OObject::Deserialize(const nlohmann::json& json)
{
    _guid = json["guid"];
    _name = json["name"];
}
