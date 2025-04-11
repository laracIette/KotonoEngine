#pragma once
#include "Object.h"
#include <unordered_set>
#include "InterfaceComponent.h"
class OInterface : public OObject
{
	BASECLASS(OObject)

public:
	void SetLayout();

private:
	std::unordered_set<RInterfaceComponent*> _components;
};

