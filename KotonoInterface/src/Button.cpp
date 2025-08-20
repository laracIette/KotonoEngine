#include "Button.h"
#include <kotono_framework/Framework.h>

WButton::WButton(const ButtonSettings& buttonSettings) :
	buttonSettings_(buttonSettings)
{
	Framework.GetInputManager().GetMouse().GetEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.AddListener(KtDelegate(this, &WButton::OnMouseLeftButtonPressed));
}

void WButton::Build(BuildSettings buildSettings)
{
	size_ = buildSettings.bounds;
	position_ = buildSettings.position;

	if (buttonSettings_.child)
	{
		buttonSettings_.child->Build(buildSettings);
	}
}

void WButton::Destroy()
{
	if (buttonSettings_.child)
	{
		buttonSettings_.child->Destroy();
	}
}

void WButton::OnMouseLeftButtonPressed()
{
	const auto& cursorPos{ Framework.GetInputManager().GetMouse().GetCursorPosition() };
	const auto distance{ glm::abs(position_ - cursorPos) };
	const auto maxDistance{ size_ / 2.0f };

	if (distance.x > maxDistance.x || distance.y > maxDistance.y)
	{
		return;
	}

	if (buttonSettings_.onPress)
	{
		buttonSettings_.onPress();
	}
}
