#include "ValueBoxString.h"
#include <kotono_common/log.h>
#include <kotono_core/TimeManager.h>
#include <kotono_interface/widgets.h>
#include <kotono_input/Keyboard.h>

WWidget* WValueBoxString::Build()
{ 
	holdAction_.SetActuationTime(0.5f);
	holdAction_.SetRepeatTime(0.05f);

	Keyboard.EventAnyKey(EInputState::Pressed)
		.AddListener(UDelegate(this, &WValueBoxString::OnAnyKeyPressed));
	Keyboard.EventAnyKey(EInputState::Released)
		.AddListener(UDelegate(this, &WValueBoxString::OnAnyKeyReleased));
	Keyboard.EventAnyKey(EInputState::Down)
		.AddListener(UDelegate(this, &WValueBoxString::OnAnyKeyDown));

	Keyboard.EventKey(EKey::Backspace, EInputState::Pressed)
		.AddListener(UDelegate(this, &WValueBoxString::OnKeyBackspacePressed)); 
	Keyboard.EventKey(EKey::Backspace, EInputState::Down)
		.AddListener(UDelegate(this, &WValueBoxString::OnKeyBackspaceDown));

	return new WWrap({
		.axis = WWrap::Axis::Vertical,
		.child = new WStack({
			.children = {
				new WColor({ UColor::White().WithAlpha(0.15f) }),
				new WButton({
					.onPress = [this]() {
						SetState([this]() { isSelected_ = true; });
					},
					.onPressOut = [this]() {
						SetState([this]() { isSelected_ = false; });
					},
				}),
				new WPadding({
					.padding = WPadding::Padding::All(4.0f),
					.child = new WText({
						.text = GetValue(),
						.fontSize = { 15.0f, 18.0f },
						.spacing = -5.0f,
					}),
				}),
			},
		}),
	});
}

void WValueBoxString::Cleanup()
{
	Keyboard.EventAnyKey(EInputState::Pressed)
		.RemoveListener(UDelegate(this, &WValueBoxString::OnAnyKeyPressed));
	Keyboard.EventAnyKey(EInputState::Released)
		.RemoveListener(UDelegate(this, &WValueBoxString::OnAnyKeyReleased));
	Keyboard.EventAnyKey(EInputState::Down)
		.RemoveListener(UDelegate(this, &WValueBoxString::OnAnyKeyDown));

	Keyboard.EventKey(EKey::Backspace, EInputState::Pressed)
		.RemoveListener(UDelegate(this, &WValueBoxString::OnKeyBackspacePressed));
	Keyboard.EventKey(EKey::Backspace, EInputState::Down)
		.RemoveListener(UDelegate(this, &WValueBoxString::OnKeyBackspaceDown));

	WWidget::Cleanup();
}

void WValueBoxString::OnKeyBackspacePressed()
{
	if (!isSelected_)
	{
		return;
	}

	holdAction_.Reset();
}

void WValueBoxString::OnKeyBackspaceDown()
{
	if (!isSelected_)
	{
		return;
	}

	if (holdAction_.Update(TimeManager.Delta()))
	{
		SetState([this]()
		{
			std::string& value{ GetValue() };
			if (!value.empty())
			{
				value.pop_back();
			}
		});
	}
}

void WValueBoxString::OnAnyKeyPressed(const EKey key)
{
	if (!isSelected_)
	{
		return;
	}

	const char character{ keyToChar(key) };

	if (currentWriteCharacter_ != character)
	{
		currentWriteCharacter_ = character;
		holdAction_.Reset();
	}
}

void WValueBoxString::OnAnyKeyReleased(const EKey key)
{
	if (!isSelected_)
	{
		return;
	}

	const char character{ keyToChar(key) };

	if (currentWriteCharacter_ == character)
	{
		currentWriteCharacter_ = 0;
	}
}

void WValueBoxString::OnAnyKeyDown(const EKey key)
{
	const char character{ keyToChar(key) };

	if (!isSelected_)
	{
		return;
	}

	if (currentWriteCharacter_ != character)
	{
		return;
	}

	if (!isalpha(character))
	{
		return;
	}

	if (holdAction_.Update(TimeManager.Delta()))
	{
		SetState([this, character]()
		{
			GetValue().push_back(character);
		});
	}
}
