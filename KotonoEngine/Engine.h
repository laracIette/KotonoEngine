#pragma once
class KTime;
class KObjectManager;
class KEngine
{
public:
	void Init();
	void Cleanup();

	KTime& GetTime();
	KObjectManager& GetObjectManager();
};

extern KEngine Engine;