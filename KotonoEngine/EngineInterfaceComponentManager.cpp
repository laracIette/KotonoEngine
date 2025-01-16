#include "EngineInterfaceComponentManager.h"
#include "EICViewport.h"
#include "EICFileExplorer.h"
#include "EICObjectExplorer.h"
#include <algorithm>

EngineInterfaceComponentManager::EngineInterfaceComponentManager()
{
	_components[0] = new EICViewport();
	_components[1] = new EICFileExplorer();
	_components[2] = new EICObjectExplorer();
}

EngineInterfaceComponentManager::~EngineInterfaceComponentManager()
{
	for (auto* component : _components)
	{
		delete component;
	}
}

void EngineInterfaceComponentManager::Update(const float deltaTime)
{
}

void EngineInterfaceComponentManager::Draw() const
{
	_layout.Draw();
}

EngineInterfaceComponent* EngineInterfaceComponentManager::GetActiveComponent() const
{
	auto it = std::find_if(
		std::begin(_components), std::end(_components),
		[](const EngineInterfaceComponent* component)
		{
			return component->GetIsActive();
		}
	);

	return (it != std::end(_components)) ? *it : nullptr;
}
