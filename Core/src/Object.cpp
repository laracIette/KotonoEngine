#include "Object.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include <kotono_common/Path.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>

#ifndef NDEBUG
std::unordered_set<ObjectPtr> KObject::debugRegistry_{};
#endif 

KObject::KObject()
    : ptrOwner_{ new UPtrOwner{} }
    , guid_{}
    , name_{}
{
    ptrOwner_->Set(this);

#ifndef NDEBUG
    debugRegistry_.insert(Ptr());
#endif
}

KObject::~KObject()
{
#ifndef NDEBUG
    debugRegistry_.erase(Ptr());
#endif

    delete ptrOwner_;
}

void KObject::PostConstruct()
{
    type_ = TypeName();
    if (name_.empty())
    {
        SetName(std::format("{0}_{1}", type_, GetGuid().ToString()));
    }
}

std::string KObject::TypeName() const
{
    std::string_view name{ typeid(*this).name() };
    return std::string{ name.substr(6) };
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

KObject::operator std::string() const
{
    return ToString();
}

UPtr<KObject> KObject::Deserialize(const nlohmann::json& json)
{
    UGuid guid{};
    UDeserialize<UGuid>{}(json, guid);
    return SObjectFactory::Get(guid);
}

#ifndef NDEBUG
void KObject::CheckDebugRegistry()
{
    if (!debugRegistry_.empty())
    {
        for (auto& object : debugRegistry_)
        {
            if (object)
            {
                KT_LOG(ELogImportanceLevel::High, "Object"
                    , "{0:48s} | L{1:03d}: {2}"
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
