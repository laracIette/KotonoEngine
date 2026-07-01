#include "ObjectManager.h"
#include "Object.h"

UPtr<KObject>& GObjectManager::GetSelectedObject()
{
	return selectedObject_;
}

void GObjectManager::SetSelectedObject(const UPtr<KObject>& object)
{
	if (selectedObject_ == object)
	{
		return;
	}

	selectedObject_ = object;
	eventSelectedObjectChanged_.Broadcast(object);
}

UEvent<UPtr<KObject>>& GObjectManager::EventSelectedObjectChanged()
{
	return eventSelectedObjectChanged_;
}
