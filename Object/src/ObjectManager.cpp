#include "ObjectManager.h"
#include "Object.h"
#include <kotono_common/log.h>
#include <kotono_input/Keyboard.h>
#include <kotono_platform/Window.h>

void SObjectManager::Init()
{
	Keyboard.EventKey(EKey::Escape, EInputState::Pressed).AddListener(this, &SObjectManager::Quit);
}

void SObjectManager::Cleanup()
{
	Keyboard.EventKey(EKey::Escape, EInputState::Pressed).RemoveListener(this, &SObjectManager::Quit);
}

void SObjectManager::Quit()
{
	Window.SetShouldClose(true);
}

UPtr<KObject>& SObjectManager::GetSelectedObject()
{
	return selectedObject_;
}

void SObjectManager::SetSelectedObject(const UPtr<KObject>& object)
{
	if (selectedObject_ == object)
	{
		return;
	}

	selectedObject_ = object;
	eventSelectedObjectChanged_.Broadcast();
}

UEvent<>& SObjectManager::EventSelectedObjectChanged()
{
	return eventSelectedObjectChanged_;
}
