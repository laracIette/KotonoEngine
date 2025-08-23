#include "Button.h"
#include "View.h"
#include <kotono_framework/Framework.h>

WButton::WButton(const ButtonSettings& buttonSettings) :
	buttonSettings_(buttonSettings)
{
	Framework.GetInputManager().GetMouse().GetEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.AddListener(KtDelegate(this, &WButton::OnMouseLeftButtonPressed));
}

void WButton::Destroy()
{
	Framework.GetInputManager().GetMouse().GetEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.RemoveListener(KtDelegate(this, &WButton::OnMouseLeftButtonPressed));

	WWidget::Destroy();
}

void WButton::OnMouseLeftButtonPressed()
{
	const auto& cursorPos{ Framework.GetInputManager().GetMouse().GetCursorPosition() };
	const auto distance{ glm::abs(GetPosition() - cursorPos) };
	const auto maxDistance{ GetSize() / 2.0f };

	if (distance.x > maxDistance.x || distance.y > maxDistance.y)
	{
		return;
	}

	if (buttonSettings_.onPress)
	{
		buttonSettings_.onPress();
	}
}
