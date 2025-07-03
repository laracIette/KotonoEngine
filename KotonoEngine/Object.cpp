#include "Object.h"
#include <nlohmann/json.hpp>
#include <kotono_framework/Serializer.h>
#include <regex>
#include "Engine.h"
#include "ObjectManager.h"
#include "Timer.h"

void KObject::Construct()
{
    name_ = GetTypeName();
}

void KObject::Init()
{
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
    return std::format("{}_{}",
        std::regex_replace(typeid(*this).name(), std::regex(R"(^(class ))"), ""),
        static_cast<std::string>(guid_)
    );
}

void KObject::SetName(const std::string& name)
{
    name_ = name;
}

void KObject::SetIsConstructed(const bool isConstructed)
{
    isConstructed_ = isConstructed;
}

void KObject::SetPath(const std::filesystem::path& path)
{
    path_ = path;
}

void KObject::SetIsDelete(const bool isDelete)
{
    isDelete_ = isDelete;
}

void KObject::Serialize() const
{
    nlohmann::json json;
    KtSerializer serializer;
    SerializeTo(json);
    serializer.WriteData(path_, json);
}

void KObject::Deserialize()
{
    nlohmann::json json;
    KtSerializer serializer;
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
