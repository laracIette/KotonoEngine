#include "Engine.h"

void SEngine::Init()
{
    timeManager_.Init();
    objectManager_.Init();
    visualizer_.Init();
    interface_.Init();
}

void SEngine::Update()
{
    timeManager_.Update();
    objectManager_.Update();
    interfacePhysicsManager_.Update();
}

void SEngine::Cleanup()
{
    interface_.Cleanup();
    timeManager_.Cleanup();
    objectManager_.Cleanup();
    visualizer_.Cleanup();
}

STimeManager& SEngine::GetTimeManager()
{
    return timeManager_;
}

const STimeManager& SEngine::GetTimeManager() const
{
    return timeManager_;
}

SObjectManager& SEngine::GetObjectManager()
{
    return objectManager_;
}

const SObjectManager& SEngine::GetObjectManager() const
{
    return objectManager_;
}

SVisualizer& SEngine::GetVisualizer()
{
    return visualizer_;
}

const SVisualizer& SEngine::GetVisualizer() const
{
    return visualizer_;
}

SInterface& SEngine::GetInterface()
{
    return interface_;
}

const SInterface& SEngine::GetInterface() const
{
    return interface_;
}

SInterfacePhysicsManager& SEngine::GetInterfacePhysicsManager()
{
    return interfacePhysicsManager_;
}

const SInterfacePhysicsManager& SEngine::GetInterfacePhysicsManager() const
{
    return interfacePhysicsManager_;
}
