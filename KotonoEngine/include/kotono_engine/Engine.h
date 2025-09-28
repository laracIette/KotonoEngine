#pragma once
class STimeManager;
class SObjectManager;
class SVisualizer;
class SInterface;
class SInterfacePhysicsManager;
class SEngine final
{
public:
	void Init();
	void Update();
	void Cleanup();

public:
	STimeManager& GetTimeManager();
	const STimeManager& GetTimeManager() const;
	SObjectManager& GetObjectManager();
	const SObjectManager& GetObjectManager() const;
	SVisualizer& GetVisualizer();
	const SVisualizer& GetVisualizer() const;
	SInterface& GetInterface();
	const SInterface& GetInterface() const;
	SInterfacePhysicsManager& GetInterfacePhysicsManager();
	const SInterfacePhysicsManager& GetInterfacePhysicsManager() const;
};

inline SEngine Engine;