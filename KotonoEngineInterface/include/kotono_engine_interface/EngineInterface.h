#pragma once
class WWidget;
class SEngineInterface final
{
public:
	void Init();
	void Cleanup();

private:
	WWidget* widget_;
};

inline SEngineInterface EngineInterface;