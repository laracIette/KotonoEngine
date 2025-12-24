#include "Game.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "ProjectSettings.h"
#include "Scene.h"
#include <kotono_common/log.h>
#include <kotono_input/Keyboard.h>

void SGame::Init()
{
	Keyboard.EventKey(EKey::S, EInputState::Pressed)
        .AddListener(KtDelegate(this, &SGame::OnKeySPressed));

    OpenStartupScene();
}

void SGame::Update()
{
    ObjectManager.InitObjects();
    if (scene_)
    {
        scene_->UpdateSceneObjects();
    }
}

void SGame::OpenScene(const UPtr<KScene>& scene)
{
    scene_ = scene;
}

void SGame::OpenInterface(const UPtr<KInterface>& interface)
{
    interface_ = interface;
}

void SGame::OpenStartupScene()
{
    if (scene_)
    {
        scene_->Delete();
    }
    const auto startupScene{ SProjectSettings::Get<std::string>("/startupScene") };
    if (scene_ = TryCast<KScene>(ObjectFactory.Get(startupScene)))
    {
        scene_->SpawnSceneObjects();
    }
}

const UPtr<KScene>& SGame::GetOpenedScene() const
{
    return scene_;
}

void SGame::OnKeySPressed() const
{
    if (!Keyboard.KeyState(EKey::LeftControl, EInputState::Down))
    {
        return;
    }

    if (scene_)
    {
        scene_->Serialize();
        KT_LOG(ELogImportanceLevel::High, "Core.SGame::OnKeySPressed()", "scene serialized");
    }
}
