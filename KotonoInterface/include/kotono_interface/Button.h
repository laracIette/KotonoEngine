#pragma once
#include "Widget.h"
#include <functional>
class WButton : public WWidget
{
public:
	struct ButtonSettings
	{
		std::function<void()> onPress;
		WWidget* child;
	};

	WButton(const ButtonSettings& buttonSettings);

	void Build(BuildSettings buildSettings) override;
	void Destroy() override;

protected:
	ButtonSettings buttonSettings_;

private:
	void OnMouseLeftButtonPressed();
};

