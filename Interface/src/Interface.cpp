#include "Interface.h"
#include "Button.h"
#include <kotono_input/Mouse.h>
#include <algorithm>

void GInterface::Init()
{
	Mouse.EventButton(EButton::Left, EInputState::Pressed).AddListener(this, &GInterface::OnMouseLeftButtonPressed);
	Mouse.EventButton(EButton::Left, EInputState::Released).AddListener(this, &GInterface::OnMouseLeftButtonReleased);
}

void GInterface::AddButton(const UPtr<WButton>& button)
{
	buttons_.Add(button);
}

void GInterface::RemoveButton(const UPtr<WButton>& button)
{
	buttons_.Remove(button);
}

void GInterface::OnMouseLeftButtonPressed() const
{
	auto buttons{ buttons_ };
	std::ranges::sort(buttons, std::ranges::greater{}, &WButton::GetLayer);

	bool hasInteracted{ false };
	for (auto& button : buttons)
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

	for (auto& button : buttons)
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
