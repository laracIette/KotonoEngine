#include "Object.h"
#include <nlohmann/json.hpp>
#include <kotono_framework/Serializer.h>
#include <regex>
#include "Engine.h"
#include "ObjectManager.h"
#include "Timer.h"

void KObject::Construct()
{
}

void KObject::Init()
{
    _name = GetTypeName();
}

void KObject::Update() 
{
}

void KObject::Cleanup() 
{
    UnlistenEvents();

    for (auto* object : objects_)
    {
        object->SetIsDelete(true);
    }
}

const UGuid& KObject::GetGuid() const
{
    return _guid;
}

const std::filesystem::path& KObject::GetPath() const
{
    return _path;
}

const std::string& KObject::GetName() const
{
    return _name;
}

const bool KObject::GetIsDelete() const
{
    return _isDelete;
}

const std::string KObject::GetTypeName() const
{
    return std::regex_replace(typeid(*this).name(), std::regex(R"(^(class ))"), "");
}

void KObject::SetName(const std::string& name)
{
    _name = name;
}

void KObject::SetPath(const std::filesystem::path& path)
{
    _path = path;
}

void KObject::SetIsDelete(const bool isDelete)
{
    _isDelete = isDelete;
}

void KObject::Serialize() const
{
    nlohmann::json json;
    KtSerializer serializer;
    SerializeTo(json);
    serializer.WriteData(_path, json);
}

void KObject::Deserialize()
{
    nlohmann::json json;
    KtSerializer serializer;
    serializer.ReadData(_path, json);
    DeserializeFrom(json);
}

void KObject::SerializeTo(nlohmann::json& json) const
{
    json["guid"] = _guid;
    json["type"] = GetTypeName();
    json["name"] = _name;
}

void KObject::DeserializeFrom(const nlohmann::json& json)
{
    _guid = json["guid"];
    _name = json["name"];
}

void KObject::Repeat(const KtDelegate<>& delegate, float frequency)
{
    auto* timer = AddObject<KTimer>();
    timer->SetDuration(frequency);
    timer->SetIsRepeat(true);
    timer->GetEventCompleted().AddListener(delegate);
    timer->Start();
}

void KObject::AddObject(KObject* object)
{
    Engine.GetObjectManager().Register(object);
    objects_.insert(object);
}

void KObject::UnlistenEvents()
{
    for (auto& [event, count] : listenedEvents_)
    {
        if (event)
        {
            event->RemoveListener(this);
        }
    }

    listenedEvents_.clear();
}
