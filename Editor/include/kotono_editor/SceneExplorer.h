#pragma once
#include <kotono_interface/Widget.h>

class WSceneExplorer : public WWidget
{
public:
	WWidget* Build() override;
	void Cleanup() override;

private:
	float offset_;
};