#include "Object.h"
#include <nlohmann/json.hpp>
#include <kotono_io/Serializer.h>
#include "ObjectManager.h"
#include "Timer.h"
#include <kotono_common/log.h>
#include <kotono_common/Path.h>

KObject::KObject(UPtrOwnerBase* ptrOwner) :
    ptrOwner_(ptrOwner),
    name_(guid_)
{
    ptrOwner_->Set(this);
}

void KObject::Init()
{
    type_ = TypeName();
    isConstructed_ = true; // todo: maybe move that
}

void KObject::Cleanup() 
{
    eventCleanup_.Broadcast();
}

const UGuid& KObject::Guid() const
{
    return guid_;
}

const std::type_info& KObject::Type() const
{
    return typeid(*this);
}

bool KObject::IsConstructed() const
{
    return isConstructed_;
}

const std::filesystem::path KObject::Path() const
{
    return KtPath::Project() / "assets" / "objects" / std::format("{}.kobject", guid_.ToString());
}

const std::string& KObject::GetName() const
{
    return name_;
}

std::string KObject::TypeName() const
{
    std::string_view name{ Type().name() };
    return std::string(name.substr(6));
}

KtEvent<>& KObject::GetEventCleanup()
{
    return eventCleanup_;
}

nlohmann::json KObject::ReadJson() const
{
    nlohmann::json json{};
    KtSerializer::Deserialize(json, Path());
    return json;
}

nlohmann::json KObject::WriteJson() const
{
    nlohmann::json json{};
    SerializeTo(json);
    return json;
}

void KObject::SetName(const std::string& name)
{
    name_ = name;
}

void KObject::Delete() const
{
    ObjectManager.Delete(ptrOwner_);
}

void KObject::Serialize() const
{
    KtSerializer::Serialize(WriteJson(), Path());
}

void KObject::Deserialize()
{
    DeserializeFrom(ReadJson());
}

std::string KObject::ToString() const
{
    return name_;
}
