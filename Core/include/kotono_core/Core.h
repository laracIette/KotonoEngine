#pragma once
class SCore final
{
public:
	void Init();
	void Update();
	void Cleanup();

private:
	void LogUPS() const;
};

inline SCore Core;