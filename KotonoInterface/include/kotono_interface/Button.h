#pragma once
#include "Widget.h"
#include <functional>
class WButton : public WWidget
{
public:
	struct ButtonSettings
	{
		/// default = {}
		std::function<void()> onPress{};
	};

	/// Set the widget's bounds as pressable
	WButton(const ButtonSettings& buttonSettings);

	VView* CreateView() override;

private:
	ButtonSettings buttonSettings_;

	void OnMouseLeftButtonPressed();
};

