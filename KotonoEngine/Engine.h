#pragma once
class KEngine
{
public:
	void Init();
	void Cleanup();

	class KTime& GetTime();
	class KObjectManager& GetObjectManager();
	class KVisualizer& GetVisualizer();
};

extern KEngine Engine;