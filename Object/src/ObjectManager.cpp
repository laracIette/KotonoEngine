#include "ObjectManager.h"
#include "Object.h"

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
	eventSelectedObjectChanged_.Broadcast(object);
}

UEvent<UPtr<KObject>>& SObjectManager::EventSelectedObjectChanged()
{
	return eventSelectedObjectChanged_;
}
