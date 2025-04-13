#pragma once
class KTime;
class KObjectManager;
class KVisualizer;
class KEngine
{
public:
	void Init();
	void Cleanup();

	KTime& GetTime();
	KObjectManager& GetObjectManager();
	KVisualizer& GetVisualizer();
};

extern KEngine Engine;