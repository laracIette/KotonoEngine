#include "ObjectFactory.h"
#include <kotono_common/Path.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>
#include "ObjectManager.h"
#include <kotono_common/log.h>
#include "Interface.h"
#include "InterfaceBoxComponent.h"
#include "InterfaceButtonComponent.h"
#include "InterfaceColliderComponent.h"
#include "InterfaceComponent.h"
#include "InterfaceImageComponent.h"
#include "InterfaceImageObject.h"
#include "InterfaceObject.h"
#include "Object.h"
#include "Scene.h"
#include "SceneComponent.h"
#include "SceneMeshComponent.h"
#include "SceneObject.h"

#define OBJECT_FACTORY(Type) { #Type, []() { return ObjectManager.Create<Type>(); } }

#define KT_LOG_IMPORTANCE_LEVEL_OBJECT_FACTORY ELogImportanceLevel::High

SObjectFactory::SObjectFactory() :
    objectFactories_({
        OBJECT_FACTORY(KInterface),
        OBJECT_FACTORY(KInterfaceBoxComponent),
        OBJECT_FACTORY(KInterfaceButtonComponent),
        OBJECT_FACTORY(KInterfaceColliderComponent),
        OBJECT_FACTORY(KInterfaceComponent),
        OBJECT_FACTORY(KInterfaceImageComponent),
        OBJECT_FACTORY(RInterfaceImageObject),
        OBJECT_FACTORY(RInterfaceObject),
        OBJECT_FACTORY(KObject),
        OBJECT_FACTORY(KScene),
        OBJECT_FACTORY(KSceneComponent),
        OBJECT_FACTORY(KSceneMeshComponent),
        OBJECT_FACTORY(TSceneObject),
    })
{
}

UPtr<KObject> SObjectFactory::Get(const UGuid& guid)
{
	// Check if already in registry
	const auto registryIt{ registry_.find(guid) };
	if (registryIt != registry_.end())
	{
		if (UPtr object{ registryIt->second })
		{
			KT_LOG(KT_LOG_IMPORTANCE_LEVEL_OBJECT_FACTORY, "Core.SObjectFactory::Get()", "found object %s", object->GetName().c_str());
			return object;
		}
	}

	const auto path{ KtPath::Project() / "assets" / "objects" / std::format("{}.kobject", guid.ToString()) };

	// Add to registry
	nlohmann::json json{};
	KtSerializer::Deserialize(json, path);

	if (!json.contains("type_"))
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_OBJECT_FACTORY, "Core.SObjectFactory::Get()", "missing element type_ in json");
		return nullptr;
	}

	const auto type{ json.at("type_").get<std::string>() };

	if (UPtr object{ GetFactory(type) })
	{
		KT_LOG(KT_LOG_IMPORTANCE_LEVEL_OBJECT_FACTORY, "Core.SObjectFactory::Get()", "created object %s", object->GetName().c_str());
		object->guid_ = guid;
		object->Deserialize();
		registry_[guid] = object;
		return object;
	}

	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_OBJECT_FACTORY, "Core.SObjectFactory::Get()", "missing value for type %s in object factories", type.c_str());
	return nullptr;
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
