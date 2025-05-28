#pragma once

class STime;
class SObjectManager;
class SVisualizer;

class SEngine final
{
public:
	void Init();
	void Cleanup();

	STime& GetTime();
	const STime& GetTime() const;
	SObjectManager& GetObjectManager();
	const SObjectManager& GetObjectManager() const;
	SVisualizer& GetVisualizer();
	const SVisualizer& GetVisualizer() const;
// we dont use private members because we could only store them as pointers
};

extern SEngine Engine;