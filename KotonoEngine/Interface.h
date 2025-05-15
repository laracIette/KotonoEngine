#pragma once
#include "Object.h"
#include <unordered_set>

class RInterfaceComponent;

class KInterface : public KObject
{
	BASECLASS(KObject)

public:
	void Init() override;

	void SetLayout();

private:
	std::unordered_set<RInterfaceComponent*> _interfaceComponents;
};

