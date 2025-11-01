#pragma once
class STimeManager;
class SObjectManager;
class SVisualizer;
class SGame;
class SInterfacePhysicsManager;
class SEngine final
{
public:
	void Init();
	void Update();
	void Cleanup();

public:
	STimeManager& TimeManager();
	const STimeManager& TimeManager() const;
	SObjectManager& ObjectManager();
	const SObjectManager& ObjectManager() const;
	SVisualizer& Visualizer();
	const SVisualizer& Visualizer() const;
	SGame& Game();
	const SGame& Game() const;
	SInterfacePhysicsManager& InterfacePhysicsManager();
	const SInterfacePhysicsManager& InterfacePhysicsManager() const;
};

inline SEngine Engine;