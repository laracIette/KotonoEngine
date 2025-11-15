#include "object_factories.h"
#include "Engine.h"
#include "ObjectManager.h"
#include "Object.h"
#include "SceneObject.h"
#include "SceneMeshObject.h"

#define OBJECT_FACTORY(Type) { #Type, []() { return Engine.ObjectManager().Create<Type>(); } }
const std::unordered_map<std::string_view, ObjectFactoryFunc> ObjectFactories = {
    OBJECT_FACTORY(TSceneObject),
    OBJECT_FACTORY(TSceneMeshObject),
};