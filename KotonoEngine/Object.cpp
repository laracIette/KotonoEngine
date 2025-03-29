#include <nlohmann/json.hpp>
#include "Object.h"

const std::string& OObject::GetName() const
{
    return _name;
}

const UGuid& OObject::GetGuid() const
{
    return _guid;
}

void OObject::SetName(const std::string& name)
{
    _name = name;
}

const nlohmann::json OObject::Serialize() const
{
    nlohmann::json data;

    data["guid"] = _guid;
    data["type"] = typeid(*this).name();
    data["name"] = _name;

    return data;
}

void OObject::Deserialize(const nlohmann::json& data)
{
    _guid = data["guid"];
    _name = data["name"];
}
