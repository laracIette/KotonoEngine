#pragma once
class SCore final
{
public:
	void Init();
	void Update();
	void Cleanup();
};

inline SCore Core;