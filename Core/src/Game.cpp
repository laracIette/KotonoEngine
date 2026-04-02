#include "Game.h"
#include "Interface.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "ProjectSettings.h"
#include "Scene.h"
#include <kotono_common/log.h>
#include <kotono_input/Keyboard.h>

void SGame::Init()
{
	Keyboard.EventKey(EKey::S, EInputState::Pressed).AddListener(this, &SGame::OnKeySPressed);

    OpenStartupInterface();
    OpenStartupScene();
}

void SGame::Update(const float deltaTime)
{
    if (interface_)
    {
        interface_->Update(deltaTime);
    }
    if (scene_)
    {
        scene_->Update(deltaTime);
    }
}

void SGame::Cleanup()
{
    if (interface_)
    {
        interface_->Delete();
    }
    if (scene_)
    {
        scene_->Delete();
    }
}

void SGame::OpenInterface(const UPtr<KInterface>& interface)
{
    interface_ = interface;
}

void SGame::OpenScene(const UPtr<KScene>& scene)
{
    scene_ = scene;
}

void SGame::OpenStartupInterface()
{
    interface_ = Create<KInterface>();
    interface_->SpawnInterfaceObjects();
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
        KT_LOG(ELogImportanceLevel::High, "Core", "scene serialized");
    }
}
