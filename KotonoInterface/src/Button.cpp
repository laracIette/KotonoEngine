#include "Button.h"
#include <kotono_framework/Framework.h>
#include "log.h"

WButton::WButton(const ButtonSettings& buttonSettings) :
	buttonSettings_(buttonSettings)
{
	KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "%p create", this);
	Framework.GetInputManager().GetMouse().GetEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.AddListener(KtDelegate(this, &WButton::OnMouseLeftButtonPressed));
}

void WButton::Cleanup()
{
	KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "%p delete", this);
	Framework.GetInputManager().GetMouse().GetEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.RemoveListener(KtDelegate(this, &WButton::OnMouseLeftButtonPressed));

	WWidget::Cleanup();
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
		KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "%p press", this);
		buttonSettings_.onPress();
	}
}
