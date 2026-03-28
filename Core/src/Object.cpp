#include "Object.h"
#include "ObjectManager.h"
#include <kotono_common/Path.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>
#include <kotono_common/log.h>

KObject::KObject(UPtrOwnerBase* ptrOwner) 
    : ptrOwner_(ptrOwner)
    , name_(guid_)
    , isConstructed_(false)
{
    ptrOwner_->Set(this);
}

void KObject::Cleanup() 
{
    for (const auto& function : unregisterDelegates_)
    {
        if (function)
        {
            function();
        }
    }
    unregisterDelegates_.clear();
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

UPath KObject::Path() const
{
    return "${PROJECT_DIRECTORY}/assets/objects/" + guid_.ToString() + ".kobject";
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

nlohmann::json KObject::ReadJson() const
{
    nlohmann::json json{};
    USerializer::Deserialize(json, Path());
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
    USerializer::Serialize(WriteJson(), Path());
}

void KObject::Deserialize()
{
    DeserializeFrom(ReadJson());
}

std::string KObject::ToString() const
{
    return name_;
}
