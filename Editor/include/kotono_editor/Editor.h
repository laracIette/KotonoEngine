#pragma once
#include <kotono_object/Ptr.h>
class WMainWindow;
class GEditor final
{
public:
	void Init();
	void Cleanup();

private:
	UPtr<WMainWindow> mainWindow_;
};

inline GEditor Editor;