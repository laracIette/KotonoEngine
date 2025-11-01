#pragma once
#include "Object.h"
class RInterfaceLayoutObject;
class RInterfaceImageObject;
class KInterface final : public KObject
{
	BASECLASS(KObject)

protected:
	void Init() override;

private:
	RInterfaceLayoutObject* layout_;
};

