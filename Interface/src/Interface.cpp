#include "Interface.h"
#include "Button.h"
#include <kotono_input/Mouse.h>
#include <algorithm>

void SInterface::Init()
{
	Mouse.EventButton(EButton::Left, EInputState::Pressed).AddListener(this, &SInterface::OnMouseLeftButtonPressed);
	Mouse.EventButton(EButton::Left, EInputState::Released).AddListener(this, &SInterface::OnMouseLeftButtonReleased);
}

void SInterface::AddButton(WButton* button)
{
	buttons_.Add(button);
}

void SInterface::RemoveButton(WButton* button)
{
	removes_.insert(button);
}

void SInterface::RemoveButtons()
{
	if (!removes_.empty())
	{
		buttons_.RemoveIf([this](WButton* button) { return removes_.contains(button); });
		removes_.clear();
	}
}

void SInterface::OnMouseLeftButtonPressed()
{
	RemoveButtons();

	auto buttons{ buttons_ };
	std::ranges::sort(buttons, std::ranges::greater{}, &WButton::Layer);

	bool hasInteracted{ false };
	for (auto* button : buttons)
	{
		if (removes_.contains(button))
		{
			continue;
		}

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

void SInterface::OnMouseLeftButtonReleased()
{
	RemoveButtons();

	auto buttons{ buttons_ };
	std::ranges::sort(buttons, std::ranges::greater{}, &WButton::Layer);

	for (auto* button : buttons)
	{
		if (removes_.contains(button))
		{
			continue;
		}

		if (button->ReceiveMouseLeftButtonReleased())
		{
			break;
		}
	}
}
