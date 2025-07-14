#include "Object.h"
#include <nlohmann/json.hpp>
#include <kotono_framework/Serializer.h>
#include "Engine.h"
#include "ObjectManager.h"
#include "Timer.h"
#include "log.h"

KObject::KObject()
{
    name_ = static_cast<std::string>(guid_);
}

KObject::~KObject()
{
}

void KObject::Init()
{
    isConstructed_ = true; // todo: move that
}

void KObject::Update() 
{
}

void KObject::Cleanup() 
{
    eventCleanup_.Broadcast();
}

const UGuid& KObject::GetGuid() const
{
    return guid_;
}

const bool KObject::GetIsConstructed() const
{
    return isConstructed_;
}

const std::filesystem::path& KObject::GetPath() const
{
    return path_;
}

const std::string& KObject::GetName() const
{
    return name_;
}

const bool KObject::GetIsDelete() const
{
    return isDelete_;
}

const std::string KObject::GetTypeName() const
{
    std::string_view name = typeid(*this).name();
    return std::string(name.substr(6));
}

KtEvent<>& KObject::GetEventCleanup()
{
    return eventCleanup_;
}

void KObject::SetName(const std::string& name)
{
    name_ = name;
}

void KObject::SetPath(const std::filesystem::path& path)
{
    path_ = path;
}

void KObject::Delete()
{
    isDelete_ = true;
    Engine.GetObjectManager().Delete(this);
}

void KObject::Serialize() const
{
    nlohmann::json json{};
    KtSerializer serializer{};
    SerializeTo(json);
    serializer.WriteData(path_, json);
}

void KObject::Deserialize()
{
    nlohmann::json json{};
    KtSerializer serializer{};
    serializer.ReadData(path_, json);
    DeserializeFrom(json);
}

void KObject::SerializeTo(nlohmann::json& json) const
{
    json["guid"] = guid_;
    json["type"] = GetTypeName();
    json["name"] = name_;
}

void KObject::DeserializeFrom(const nlohmann::json& json)
{
    guid_ = json["guid"];
    name_ = json["name"];
}
