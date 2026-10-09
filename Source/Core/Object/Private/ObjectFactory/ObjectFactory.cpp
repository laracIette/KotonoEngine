#include "ObjectFactory/ObjectFactory.h"

#include "Object/Object.h"
#include <Logging/log.h>
#include <Path/Path.h>
#include <Serializer/Serializer.h>
#include <nlohmann/json.hpp>

#define KT_LOG_IMPORTANCE_LEVEL_OBJECT_FACTORY ELogImportance::Medium

UMap<std::string_view, SObjectFactory::ObjectFactoryFunc> SObjectFactory::objectFactories_{};
UMap<UGuid, ObjectPtr> SObjectFactory::registry_{};

void SObjectFactory::Register(std::string_view className, ObjectFactoryFunc function)
{
	objectFactories_.TryEmplace(className, function);
}

auto SObjectFactory::Get(UGuid const& guid) -> ObjectPtr
{
	// Check if already in registry and valid
	auto const registryIt{ registry_.Find(guid) };
	if (registry_.IsValidIterator(registryIt))
	{
		if (UPtr const object{ registryIt->second })
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_OBJECT_FACTORY, "Object", "found object {0}", object->GetName());
			return object;
		}
	}

	auto const assetPath{ UPath{ "${PROJECT_DIRECTORY}/Assets/objects" } / guid.ToString() + ".kobject" };
	auto const tempPath{ UPath{ "${PROJECT_DIRECTORY}/Temp/objects" } / guid.ToString() + ".kobject" };

	// Add to registry
	nlohmann::json json{};
	SSerializer::Deserialize(json, assetPath);
	SSerializer::Serialize(json, tempPath);

	auto const it{ json.find("type_") };
	if (it == json.end())
	{
		KT_LOG_SEVERITY(ELogImportance::High, ELogSeverity::Warning, "Object", "missing element type_ in json");
		return nullptr;
	}

	auto const type{ it->get<std::string>() };
	if (UPtr const object{ GetFactory(type) })
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_OBJECT_FACTORY, "Object", "created object {0}", object->GetName());
		object->guid_ = guid;
		object->Deserialize();
		registry_.InsertOrAssign(guid, object);
		return object;
	}

	KT_LOG_SEVERITY(ELogImportance::High, ELogSeverity::Warning, "Object", "missing value for type {0} in object factories", type);
	return nullptr;
}

auto SObjectFactory::GetFactory(std::string_view typeName) -> ObjectPtr
{
    auto const it{ objectFactories_.Find(typeName) };
    if (objectFactories_.IsValidIterator(it))
    {
        return it->second();
    }
    return nullptr;
}
