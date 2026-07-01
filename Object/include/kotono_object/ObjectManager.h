#pragma once
#include "Ptr.h"
#include <kotono_common/Event.h>
#include <type_traits>
#include <unordered_set>

class KObject;

class GObjectManager final
{
public:
	UPtr<KObject>& GetSelectedObject();
	void SetSelectedObject(const UPtr<KObject>& object);
	UEvent<UPtr<KObject>>& EventSelectedObjectChanged();

private:
	UPtr<KObject> selectedObject_;
	UEvent<UPtr<KObject>> eventSelectedObjectChanged_;
};

inline GObjectManager ObjectManager;
