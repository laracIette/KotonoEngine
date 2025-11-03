#include "Game.h"
#include "Engine.h"
#include "ObjectManager.h"

void SGame::Update()
{
    Engine.ObjectManager().Update();
}

void SGame::OpenScene(const UPtr<KScene>& scene)
{
    scene_ = scene;
}

void SGame::OpenInterface(const UPtr<KInterface>& interface)
{
    interface_ = interface;
}