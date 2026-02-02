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
	buttons_.push_back(button);
}

void SInterface::RemoveButton(WButton* button)
{
	std::erase(buttons_, button);
}

void SInterface::OnMouseLeftButtonPressed()
{
	std::ranges::sort(buttons_, std::ranges::greater{}, &WButton::Layer);
	bool hasInteracted{ false };
	for (size i{ 0 }; i < buttons_.size(); ++i) // issue with deletion / additions prob
	{
		auto* button{ buttons_[i] };
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
	std::ranges::sort(buttons_, std::ranges::greater{}, &WButton::Layer);
	for (size i{ 0 }; i < buttons_.size(); ++i)
	{
		auto* button{ buttons_[i] };
		if (button->ReceiveMouseLeftButtonReleased())
		{
			break;
		}
	}
}
