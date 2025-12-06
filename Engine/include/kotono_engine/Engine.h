#pragma once
class SEngine final
{
public:
	void Init();
	void Update();
	void Cleanup();
};

inline SEngine Engine;