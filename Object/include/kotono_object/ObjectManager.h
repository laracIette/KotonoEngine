#pragma once
#include "Guid.h"
#include <kotono_common/Event.h>
#include "Ptr.h"
#include <type_traits>
#include <unordered_set>

class KObject;

class SObjectManager final
{
	friend class SCore;
	friend class SGame;

private:
	void Init();
	void Cleanup();

public:
	UPtr<KObject>& GetSelectedObject();
	void SetSelectedObject(const UPtr<KObject>& object);
	UEvent<UPtr<KObject>>& EventSelectedObjectChanged();

private:
	void Quit();

private:
	UPtr<KObject> selectedObject_;
	UEvent<UPtr<KObject>> eventSelectedObjectChanged_;
};

inline SObjectManager ObjectManager;
