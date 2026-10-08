#include "Object/Object.h"

#include "ObjectFactory/ObjectFactory.h"
#include <Serializer/Serializer.h>
#include <nlohmann/json.hpp>

#ifndef NDEBUG
std::unordered_set<ObjectPtr> KObject::debugRegistry_{};
#endif 

KObject::KObject()
    : ptr_{ this }
    , guid_{}
    , name_{}
{
#ifndef NDEBUG
    debugRegistry_.insert(Ptr());
#endif
}

KObject::~KObject()
{
#ifndef NDEBUG
    debugRegistry_.erase(Ptr());
#endif

    ptr_.Invalidate();
}

void KObject::Delete()
{
    delete this;
}

auto KObject::GetTypeName() const -> std::string
{
    std::string_view const name{ typeid(*this).name() };
    return std::string{ name.substr(6) };
}

auto KObject::GetMemberVariablePointer(size offset) const -> void*
{
    return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(this) + offset);
}

void KObject::Serialize() const
{
    nlohmann::json json{};
    SerializeTo(json);
    SSerializer::Serialize(json, GetAssetPath());
}

void KObject::Deserialize()
{
    nlohmann::json json{};
    SSerializer::Deserialize(json, GetAssetPath());
    DeserializeFrom(json);
}

auto KObject::ToString() const -> std::string
{
    return name_;
}

auto KObject::Deserialize(nlohmann::json const& json) -> ObjectPtr
{
    UGuid guid{};
    UDeserialize<UGuid>{}(json, guid);
    return SObjectFactory::Get(guid);
}

auto KObject::GetAssetPath() const -> UPath
{
    return UPath{ "${PROJECT_DIRECTORY}/Assets/objects" } / GetGuid().ToString() + ".kobject";
}

auto KObject::GetTempPath() const -> UPath
{
    return UPath{ "${PROJECT_DIRECTORY}/Temp/objects" } / GetGuid().ToString() + ".kobject";
}

#ifndef NDEBUG
void KObject::CheckDebugRegistry()
{
    if (!debugRegistry_.empty())
    {
        for (auto const& object : debugRegistry_)
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
        
        throw std::runtime_error{ "KObject::debugRegistry_ must be empty when quitting the application." };
    }
}
#endif

#include "Object.generated.inl"
