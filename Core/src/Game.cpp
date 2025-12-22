#include "Game.h"
#include "ObjectManager.h"
#include "ProjectSettings.h"
#include "Scene.h"

void SGame::Init()
{
    const auto startupScene{ SProjectSettings::Get<std::string>("/startupScene") };
    scene_ = ObjectManager.Create<KScene>();
    scene_->guid_ = startupScene;

#   if true
	    scene_->Deserialize();
#   else
        scene_->Serialize();
#   endif

    scene_->SpawnSceneObjects();
}

void SGame::Update()
{
    ObjectManager.InitObjects();
    scene_->UpdateSceneObjects();
}

void SGame::OpenScene(const UPtr<KScene>& scene)
{
    scene_ = scene;
}

void SGame::OpenInterface(const UPtr<KInterface>& interface)
{
    interface_ = interface;
}