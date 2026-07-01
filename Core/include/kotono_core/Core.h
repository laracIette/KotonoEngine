#pragma once
class GCore final
{
public:
	void Init();
	void Update();
	void Cleanup();

private:
	void LogUPS() const;
};

inline GCore Core;