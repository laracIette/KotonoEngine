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
#include "SceneMeshObject.h"
#include "SceneObject.h"

#define OBJECT_FACTORY(Type) { #Type, []() { return ObjectManager.Create<Type>(); } }

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
        OBJECT_FACTORY(TSceneMeshObject),
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
		return registryIt->second;
	}

	const auto path{ Path.Project() / "assets" / "objects" / std::format("{}.kobject", guid.ToString()) };

	// Add to registry
	nlohmann::json json{};
	KtSerializer::Deserialize(json, path);

    const auto type{ json.at("type_").get<std::string>() };

	if (const auto object{ GetFactory(type) })
	{
		object->guid_ = guid;
		object->Deserialize();
		registry_[guid] = object;
		return object;
	}

	KT_LOG(KT_LOG_COMPILE_TIME_LEVEL, "Core.SObjectFactory::Get()", "missing value for type %s in object factories", type.c_str());
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
