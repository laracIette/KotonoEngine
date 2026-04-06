#include "Object.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include <kotono_common/Path.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>

#if defined(_DEBUG)
std::unordered_set<UPtr<KObject>> KObject::debugRegistry_{};
#endif 

KObject::KObject(UPtrOwner* ptrOwner) 
    : ptrOwner_(ptrOwner)
    , name_(guid_)
    , isConstructed_(false)
{
    ptrOwner_->Set(this);

#if defined(_DEBUG)
    debugRegistry_.insert(Ptr());
#endif
}

KObject::~KObject()
{
    for (const auto& function : unregisterDelegates_)
    {
        if (function)
        {
            function();
        }
    }

#if defined(_DEBUG)
    debugRegistry_.erase(Ptr());
#endif

    delete ptrOwner_;
}

void KObject::OnConstructed()
{
    if (!isConstructed_)
    {
        isConstructed_ = true;
        type_ = TypeName();
        SetName(std::format("{0}_{1}", TypeName(), Guid().ToString()));
    }
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

void KObject::Delete()
{
    delete this;
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

UPtr<KObject> KObject::Deserialize(const nlohmann::json& json)
{
    UGuid guid{};
    UDeserialize<UGuid>{}(json, guid);
    return ObjectFactory.Get(guid);
}

#if defined(_DEBUG)
void KObject::CheckDebugRegistry()
{
    if (!debugRegistry_.empty())
    {
        throw "KObject::DebugRegistry must be empty when quitting the application.";
    }
}
#endif

#include "generated/Object.generated.inl"
