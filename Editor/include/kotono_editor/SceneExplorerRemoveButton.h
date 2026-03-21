#pragma once
#include <kotono_interface/Widget.h>
class WSceneExplorerRemoveButton final : public WWidget
{
public:
	WidgetPtr Build() override;
	void Cleanup() override;
};

