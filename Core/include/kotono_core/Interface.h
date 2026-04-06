#pragma once
#include "generated/Interface.generated.h"
#include <kotono_object/Object.h>
#include <kotono_common/Pool.h>

class RInterfaceObject;

class KInterface : public KObject
{
	GENERATED_KINTERFACE()

private:
	friend class SGame;

public:
	KInterface(UPtrOwner* ptrOwner);
	~KInterface() override;

public:
	void SpawnInterfaceObjects();

private:
	void Update(const float deltaTime);

private:
	SERIALIZE KtPool<UPtr<RInterfaceObject>> interfaceObjects_;
};

