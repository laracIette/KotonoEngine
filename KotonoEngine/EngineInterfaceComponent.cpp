#include "EngineInterfaceComponent.h"

EngineInterfaceComponent::EngineInterfaceComponent(const std::string& name) :
	_isVisible(false),
	_isActive(false),
	_rect()
{
	SetName(name);
}

const bool EngineInterfaceComponent::GetIsVisible() const
{
	return _isVisible;
}

const bool EngineInterfaceComponent::GetIsActive() const
{
	return _isActive;
}

EngineInterfaceRect& EngineInterfaceComponent::GetRect()
{
	return _rect;
}

void EngineInterfaceComponent::SetIsVisible(const bool isVisible)
{
	_isVisible = isVisible;
}

void EngineInterfaceComponent::SetIsActive(const bool isActive)
{
	_isActive = isActive;
}
