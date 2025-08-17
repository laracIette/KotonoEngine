#include "Object.h"
#include <nlohmann/json.hpp>
#include <kotono_framework/Serializer.h>
#include "Engine.h"
#include "ObjectManager.h"
#include "Timer.h"
#include "log.h"

KObject::KObject() :
    name_(static_cast<std::string>(guid_))
{
}

void KObject::Init()
{
    isConstructed_ = true; // todo: maybe move that
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

bool KObject::GetIsConstructed() const
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

bool KObject::GetIsDelete() const
{
    return isDelete_;
}

std::string KObject::GetTypeName() const
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
    if (isDelete_)
    {
        return;
    }
    isDelete_ = true;
    Engine.GetObjectManager().Delete(this);
}

void KObject::DelayDelete(const UDuration& delay)
{
    Delay(KtDelegate(this, &KObject::Delete), delay);
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

void KObject::Delay(const KtDelegate<>& delegate, const UDuration& delay) const
{
    auto* timer = Engine.GetObjectManager().Create<KTimer>();
    timer->GetEventCompleted().AddListener(KtDelegate(timer, &KTimer::Delete));
    timer->GetEventCompleted().AddListener(delegate);
    timer->SetDuration(delay);
    timer->Start();
}

void KObject::Delay(KtDelegate<>&& delegate, const UDuration& delay) const
{
    auto* timer = Engine.GetObjectManager().Create<KTimer>();
    timer->GetEventCompleted().AddListener(KtDelegate(timer, &KTimer::Delete));
    timer->GetEventCompleted().AddListener(std::move(delegate));
    timer->SetDuration(delay);
    timer->Start();
}
