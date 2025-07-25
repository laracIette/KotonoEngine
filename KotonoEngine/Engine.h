#pragma once
#include "Time.h"
#include "ObjectManager.h"
#include "Visualizer.h"
#include "Interface.h"
#include "InterfacePhysicsManager.h"

class SEngine final
{
	friend class SApplication;

private:
	void Init();
	void Update();
	void Cleanup();

public:
	STime& GetTime();
	const STime& GetTime() const;
	SObjectManager& GetObjectManager();
	const SObjectManager& GetObjectManager() const;
	SVisualizer& GetVisualizer();
	const SVisualizer& GetVisualizer() const;
	SInterface& GetInterface();
	const SInterface& GetInterface() const;
	SInterfacePhysicsManager& GetInterfacePhysicsManager();
	const SInterfacePhysicsManager& GetInterfacePhysicsManager() const;

private:
	STime time_;
	SObjectManager objectManager_;
	SVisualizer visualizer_;
	SInterface interface_;
	SInterfacePhysicsManager interfacePhysicsManager_;
};

extern SEngine Engine;