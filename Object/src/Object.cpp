#include "Object.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include <kotono_common/Path.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>

#if defined(_DEBUG)
std::unordered_set<UPtr<KObject>> KObject::debugRegistry_{};
#endif 

KObject::KObject() 
    : ptrOwner_{ new UPtrOwner{} }
    , name_{ guid_ }
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

#if defined (_DEBUG)
    debugRegistry_.erase(Ptr());
#endif

    delete ptrOwner_;
}

void KObject::PostConstruct()
{
    type_ = TypeName();
    SetName(std::format("{0}_{1}", TypeName(), GetGuid().ToString()));
}

const std::type_info& KObject::Type() const
{
    return typeid(*this);
}

std::string KObject::TypeName() const
{
    std::string_view name{ Type().name() };
    return std::string(name.substr(6));
}

UPath KObject::InstancePath() const
{
    return "${PROJECT_DIRECTORY}/assets/objects/" + GetGuid().ToString() + ".kobject";
}

nlohmann::json KObject::ReadJson() const
{
    nlohmann::json json{};
    SSerializer::Deserialize(json, InstancePath());
    return json;
}

nlohmann::json KObject::WriteJson() const
{
    nlohmann::json json{};
    SerializeTo(json);
    return json;
}

void KObject::Delete()
{
    delete this;
}

void KObject::Serialize() const
{
    SSerializer::Serialize(WriteJson(), InstancePath());
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
    return SObjectFactory::Get().Get(guid);
}

#if defined (_DEBUG)
void KObject::CheckDebugRegistry()
{
    if (!debugRegistry_.empty())
    {
        for (auto& object : debugRegistry_)
        {
            if (object)
            {
                KT_LOG(ELogImportanceLevel::High, "Object"
                    , "{0:32s} | L{1:03d}: {2}"
                    , object->ToString()
                    , object->sourceLine
                    , object->sourceFunc
                );
            }
            else
            {
                KT_LOG(ELogImportanceLevel::High, "Object", "NULL");
            }
        }
        throw "KObject::debugRegistry_ must be empty when quitting the application.";
    }
}
#endif

#include "generated/Object.generated.inl"
