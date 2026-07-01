#pragma once
#include "generated/Interface.generated.h"
#include <kotono_object/Object.h>
#include <kotono_common/Pool.h>

class RInterfaceObject;

class KInterface : public KObject
{
	GENERATED_KINTERFACE()

private:
	friend class GGame;

public:
	KInterface();
	~KInterface() override;

public:
	void SpawnInterfaceObjects();

private:
	void Update(const float deltaTime);

private:
	SERIALIZE UPool<UPtr<RInterfaceObject>> interfaceObjects_;
};

