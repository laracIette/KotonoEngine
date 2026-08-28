#include "Interface.h"

#include "Button.h"
#include <algorithm>
#include <kotono_input/Mouse.h>

void GInterface::Init()
{
	Mouse.GetEventButton(EButton::Left, EInputState::Pressed).AddListener(this, &GInterface::OnMouseLeftButtonPressed);
	Mouse.GetEventButton(EButton::Left, EInputState::Released).AddListener(this, &GInterface::OnMouseLeftButtonReleased);
}

void GInterface::AddButton(UPtr<WButton> const& button)
{
	buttons_.Add(button);
}

void GInterface::RemoveButton(UPtr<WButton> const& button)
{
	buttons_.Remove(button);
}

void GInterface::OnMouseLeftButtonPressed() const
{
	auto buttons{ buttons_ };
	std::ranges::sort(buttons, std::ranges::greater{}, &WButton::GetLayer);

	bool hasInteracted{ false };
	for (auto const& button : buttons)
	{
		if (button)
		{
			if (!hasInteracted && button->ReceiveMouseLeftButtonPressed())
			{
				hasInteracted = true;
			}
			else
			{
				button->OnMouseLeftButtonPressedNoInteract();
			}
		}
	}
}

void GInterface::OnMouseLeftButtonReleased() const
{
	auto buttons{ buttons_ };
	std::ranges::sort(buttons, std::ranges::greater{}, &WButton::GetLayer);

	for (auto const& button : buttons)
	{
		if (button)
		{
			if (button->ReceiveMouseLeftButtonReleased())
			{
				break;
			}
		}
	}
}
