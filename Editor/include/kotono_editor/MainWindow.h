#pragma once
#include "generated/MainWindow.generated.h"
#include <kotono_interface/Widget.h>
class WMainWindow : public WWidget
{
	GENERATED_WMAINWINDOW()

protected:
	WidgetPtr Build() override;

public:
	void BeginDraw();
	void EndDraw();

	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

private:
	void OnWindowResized(const glm::uvec2 extent);
};

