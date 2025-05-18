#pragma once
#include "Object.h"
#include <unordered_set>

class KInterface : public KObject
{
	BASECLASS(KObject)

public:
	void Init() override;

	void SetLayout();

private:
};

