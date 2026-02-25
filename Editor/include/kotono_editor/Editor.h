#pragma once
#include <kotono_interface/widget_fwd.h>
class SEditor final
{
public:
	void Init();
	void Update();
	void Cleanup();

private:
	WidgetPtr widget_;

	void CreateWidget();
	void DeleteWidget();

	void RefreshMainWindowWidget();
};

inline SEditor Editor;