#include "Game.h"
#include "Engine.h"
#include "ObjectManager.h"
#include "Scene.h"
#include <kotono_common/Path.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>
#include "serialize.h"

void SGame::Init()
{
	nlohmann::json json{};
	KtSerializer::Deserialize(json, Path.Project() / "config.json");

#   if false
	    deserialize(json.at("startupScene"), scene_);
#   else
        scene_ = Engine.ObjectManager().Create<KScene>();
        scene_->guid_ = "6ed943411c1d0145-fa7e129d436fefc7-d610a013cfe163f9-48ab854138be189a";
        scene_->Serialize();
#   endif

    scene_->SpawnSceneObjects();
}

void SGame::Update()
{
    Engine.ObjectManager().InitObjects();
    scene_->UpdateSceneObjects();
    Engine.ObjectManager().DeleteObjects();
}

void SGame::OpenScene(const UPtr<KScene>& scene)
{
    scene_ = scene;
}

void SGame::OpenInterface(const UPtr<KInterface>& interface)
{
    interface_ = interface;
}