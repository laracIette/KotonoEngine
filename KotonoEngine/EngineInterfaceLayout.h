#pragma once
#include "EngineInterfaceComponent.h"
#include <unordered_set>
class EngineInterfaceLayout
{
public:
	void AddComponent(EngineInterfaceComponent* component);
	void RemoveComponent(EngineInterfaceComponent* component);

	void OnComponentMoved(EngineInterfaceComponent* component);

	void Draw() const;

private:
	std::unordered_set<EngineInterfaceComponent*> _components;
};

