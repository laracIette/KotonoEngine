#pragma once
class SEngine final
{
public:
	void Init();
	void Cleanup();

	class STime& GetTime();
	class SObjectManager& GetObjectManager();
	class SVisualizer& GetVisualizer();
// we dont use private members because we could only store them as pointers
};

extern SEngine Engine;