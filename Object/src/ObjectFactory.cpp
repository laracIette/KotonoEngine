#include "ObjectFactory.h"
#include "Object.h"
#include "ObjectManager.h"
#include <kotono_common/log.h>
#include <kotono_common/Path.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>

#define KT_LOG_IMPORTANCE_LEVEL_OBJECT_FACTORY ELogImportanceLevel::Medium

SObjectFactory& SObjectFactory::Get()
{
	static SObjectFactory objectFactory{};
	return objectFactory;
}

UPtr<KObject> SObjectFactory::Get(const UGuid& guid)
{
	// Check if already in registry
	const auto registryIt{ registry_.find(guid) };
	if (registryIt != registry_.end())
	{
		if (UPtr object{ registryIt->second })
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_OBJECT_FACTORY, "Core", "found object {}", object->GetName());
			return object;
		}
	}

	const UPath path{ UPath("${PROJECT_DIRECTORY}/assets/objects") / std::format("{}.kobject", guid.ToString())};

	// Add to registry
	nlohmann::json json{};
	USerializer::Deserialize(json, path);

	const auto it{ json.find("type_") };
	if (it == json.end())
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_OBJECT_FACTORY, "Core", "missing element type_ in json");
		return nullptr;
	}

	const auto type{ it->get<std::string>() };
	if (UPtr object{ GetFactory(type) })
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_OBJECT_FACTORY, "Core", "created object {}", object->GetName());
		object->guid_ = guid;
		object->Deserialize();
		registry_[guid] = object;
		return object;
	}

	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_OBJECT_FACTORY, "Core", "missing value for type {} in object factories", type);
	return nullptr;
}

void SObjectFactory::Register(const std::string_view className, const ObjectFactoryFunc& function)
{
	objectFactories_[className] = function;
}

UPtr<KObject> SObjectFactory::GetFactory(const std::string_view typeName) const
{
    const auto it{ objectFactories_.find(typeName) };
    if (it != objectFactories_.end())
    {
        return it->second();
    }
    return nullptr;
}

UAutoRegister::UAutoRegister(const std::string_view className, const SObjectFactory::ObjectFactoryFunc& creator)
{
	SObjectFactory::Get().Register(className, creator);
}
