#include "Engine.h"

SEngine Engine;

void SEngine::Init()
{
    time_.Init();
    objectManager_.Init();
    visualizer_.Init();
    interface_.Init();
}

void SEngine::Update()
{
    time_.Update();
    objectManager_.Update();
    interfacePhysicsManager_.Update();
}

void SEngine::Cleanup()
{
    time_.Cleanup();
    objectManager_.Cleanup();
    visualizer_.Cleanup();
}

STime& SEngine::GetTime()
{
    return time_;
}

const STime& SEngine::GetTime() const
{
    return time_;
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
