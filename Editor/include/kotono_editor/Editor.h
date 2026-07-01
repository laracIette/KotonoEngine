#pragma once
#include <kotono_object/Ptr.h>
class WMainWindow;
class GEditor final
{
public:
	void Init();
	void Update();
	void Cleanup();

private:
	UPtr<WMainWindow> mainWindow_;

	void CreateWidget();
	void DeleteWidget();

	void RefreshMainWindowWidget();
};

inline GEditor Editor;