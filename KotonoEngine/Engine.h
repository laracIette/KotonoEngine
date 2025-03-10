#pragma once
#include "Time.h"
#include "ObjectManager.h"
class KEngine
{
public:
	void Init();
	void Cleanup();

	KTime& GetTime();
	KObjectManager& GetObjectManager();

private:
	KTime _time;
	KObjectManager _objectManager;
};

extern KEngine Engine;