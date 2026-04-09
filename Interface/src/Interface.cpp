#include "Interface.h"
#include "Button.h"
#include <kotono_input/Mouse.h>
#include <algorithm>

void SInterface::Init()
{
	Mouse.EventButton(EButton::Left, EInputState::Pressed).AddListener(this, &SInterface::OnMouseLeftButtonPressed);
	Mouse.EventButton(EButton::Left, EInputState::Released).AddListener(this, &SInterface::OnMouseLeftButtonReleased);
}

void SInterface::AddButton(const UPtr<WButton>& button)
{
	buttons_.Add(button);
}

void SInterface::RemoveButton(const UPtr<WButton>& button)
{
	buttons_.Remove(button);
}

void SInterface::OnMouseLeftButtonPressed() const
{
	auto buttons{ buttons_ };
	std::ranges::sort(buttons, std::ranges::greater{}, &WButton::Layer);

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

void SInterface::OnMouseLeftButtonReleased() const
{
	auto buttons{ buttons_ };
	std::ranges::sort(buttons, std::ranges::greater{}, &WButton::Layer);

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
