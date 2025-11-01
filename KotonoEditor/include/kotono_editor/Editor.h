#pragma once
class WWidget;
class SEditor final
{
public:
	void Init();
	void Cleanup();

private:
	WWidget* widget_;
};

inline SEditor Editor;