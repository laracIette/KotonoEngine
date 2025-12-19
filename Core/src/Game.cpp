#include "Game.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "ProjectSettings.h"
#include <kotono_common/Path.h>
#include "serialize.h"

void SGame::Init()
{
    const auto startupScene{ ProjectSettings.Get<std::string>("/startupScene") };

#   if false
	    deserialize(startupScene, scene_);
#   else
        scene_ = ObjectManager.Create<KScene>();
        scene_->guid_ = startupScene;
        //scene_->Serialize();
#   endif

    scene_->SpawnSceneObjects();
}

void SGame::Update()
{
    ObjectManager.InitObjects();
    scene_->UpdateSceneObjects();
    ObjectManager.DeleteObjects();
}

void SGame::OpenScene(const UPtr<KScene>& scene)
{
    scene_ = scene;
}

void SGame::OpenInterface(const UPtr<KInterface>& interface)
{
    interface_ = interface;
}