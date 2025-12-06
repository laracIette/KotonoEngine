#pragma once
#include "generated/Interface.generated.h"
#include "Object.h"
class RInterfaceImageObject;
class KInterface : public KObject
{
	GENERATED_KINTERFACE()

protected:
	void Init() override;
};

