#pragma once
class WWidget;
class SEditor final
{
public:
	void Init();
	void Update();
	void Cleanup();

private:
	WWidget* widget_;

	void CreateWidget();
	void DeleteWidget();

	void RefreshMainWindowWidget();
};

inline SEditor Editor;