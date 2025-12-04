#pragma once
class STimeManager;
class SObjectManager;
class SObjectFactory;
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
	SObjectFactory& ObjectFactory();
	const SObjectFactory& ObjectFactory() const;
	SGame& Game();
	const SGame& Game() const;
	SInterfacePhysicsManager& InterfacePhysicsManager();
	const SInterfacePhysicsManager& InterfacePhysicsManager() const;
};

inline SEngine Engine;