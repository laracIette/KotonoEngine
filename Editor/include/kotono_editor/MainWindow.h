#pragma once
#include "generated/MainWindow.generated.h"
#include <kotono_interface/Widget.h>
class WMainWindow : public WWidget
{
	GENERATED_WMAINWINDOW()

public:
	void BeginDraw();
	void EndDraw();

	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

protected:
	WidgetPtr Build() override;

private:
	void OnWindowResized(const glm::uvec2 extent);
};

