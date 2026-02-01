#include "Interface.h"
#include "Button.h"
#include <kotono_input/Mouse.h>
#include <algorithm>

void SInterface::Init()
{
	Mouse.EventButton(EButton::Left, EInputState::Pressed)
		.AddListener(UDelegate(this, &SInterface::OnMouseLeftButtonPressed));
	Mouse.EventButton(EButton::Left, EInputState::Released)
		.AddListener(UDelegate(this, &SInterface::OnMouseLeftButtonReleased));
}

void SInterface::AddButton(WButton* button)
{
	buttons_.push_back(button);
}

void SInterface::RemoveButton(WButton* button)
{
	std::erase(buttons_, button);
}

void SInterface::OnMouseLeftButtonPressed()
{
	std::ranges::sort(buttons_, std::ranges::less{}, &WButton::Layer);
	for (auto* button : buttons_)
	{
		if (button->ReceiveMouseLeftButtonPressed())
		{
			break;
		}
	}
}

void SInterface::OnMouseLeftButtonReleased()
{
	std::ranges::sort(buttons_, std::ranges::less{}, &WButton::Layer);
	for (auto* button : buttons_)
	{
		if (button->ReceiveMouseLeftButtonReleased())
		{
			break;
		}
	}
}
