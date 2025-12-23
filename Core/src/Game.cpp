#include "Game.h"
#include "ObjectManager.h"
#include "ProjectSettings.h"
#include "Scene.h"
#include <kotono_common/log.h>
#include <kotono_input/InputManager.h>
#include <kotono_input/Keyboard.h>

void SGame::Init()
{
	InputManager.Keyboard().EventKey(KT_KEY_S, KT_INPUT_STATE_PRESSED)
        .AddListener(KtDelegate(this, &SGame::OnKeySPressed));

    const auto startupScene{ SProjectSettings::Get<std::string>("/startupScene") };
    scene_ = ObjectManager.Create<KScene>();
    scene_->guid_ = startupScene;

	scene_->Deserialize();
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

const UPtr<KScene>& SGame::GetOpenedScene() const
{
    return scene_;
}

void SGame::OnKeySPressed() const
{
    if (!InputManager.Keyboard().KeyState(KT_KEY_LEFT_CONTROL, KT_INPUT_STATE_DOWN))
    {
        return;
    }

	scene_->Serialize();

	KT_LOG(KT_LOG_IMPORTANCE_LEVEL_HIGH, "Core.SGame::OnKeySPressed()", "scene serialized");
}
