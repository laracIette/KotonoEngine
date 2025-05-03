#include "Object.h"
#include <nlohmann/json.hpp>
#include <kotono_framework/Serializer.h>
#include <regex>

void OObject::Init()
{
    _name = GetTypeName();
}

void OObject::Update() 
{
}

void OObject::Cleanup() 
{
    for (auto* event : _listenedEvents)
    {
        if (event)
        {
            event->RemoveListener(this);
        }
    }
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

const std::string OObject::GetTypeName() const
{
    return std::regex_replace(typeid(*this).name(), std::regex(R"(^(class ))"), "");
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
    SerializeTo(json);
    serializer.WriteData(_path, json);
}

void OObject::Deserialize()
{
    nlohmann::json json;
    KtSerializer serializer;
    serializer.ReadData(_path, json);
    DeserializeFrom(json);
}

void OObject::SerializeTo(nlohmann::json& json) const
{
    json["guid"] = _guid;
    json["type"] = GetTypeName();
    json["name"] = _name;
}

void OObject::DeserializeFrom(const nlohmann::json& json)
{
    _guid = json["guid"];
    _name = json["name"];
}
