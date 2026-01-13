#include "ValueBoxString.h"
#include <kotono_core/TimeManager.h>
#include <kotono_interface/widgets.h>
#include <kotono_input/Keyboard.h>

static constexpr f32 MIN_HOLD_TIME{ 0.5f };
static constexpr f32 DELETE_FREQUENCY{ 0.05f };

WWidget* WValueBoxString::Build()
{
	Keyboard.EventKey().AddListener(UDelegate(this, &WValueBoxString::OnKey));

	Keyboard.EventKey(EKey::Backspace, EInputState::Down)
		.AddListener(UDelegate(this, &WValueBoxString::OnKeyBackspaceDown));
	Keyboard.EventKey(EKey::Backspace, EInputState::Released)
		.AddListener(UDelegate(this, &WValueBoxString::OnKeyBackspaceReleased));

	return new WBox({
		.size = { 300.0f, 26.0f },
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
						.fontSize = { 14.0f, 18.0f },
						.spacing = -5.0f,
					}),
				}),
			},
		}),
	});
}

void WValueBoxString::Cleanup()
{
	Keyboard.EventKey().RemoveListener(UDelegate(this, &WValueBoxString::OnKey));

	Keyboard.EventKey(EKey::Backspace, EInputState::Down)
		.RemoveListener(UDelegate(this, &WValueBoxString::OnKeyBackspaceDown));
	Keyboard.EventKey(EKey::Backspace, EInputState::Released)
		.RemoveListener(UDelegate(this, &WValueBoxString::OnKeyBackspaceReleased));

	WWidget::Cleanup();
}

void WValueBoxString::OnKeyBackspaceDown()
{
	if (!isSelected_)
	{
		return;
	}

	keyHeldTime_ += TimeManager.Delta();

	if (isKeyHeld_ && keyHeldTime_ > DELETE_FREQUENCY)
	{
		keyHeldTime_ = 0.0f;
	}
	else if (keyHeldTime_ > MIN_HOLD_TIME)
	{
		isKeyHeld_ = true;
		keyHeldTime_ = 0.0f;
	}
	else if (wasKeyDown_)
	{
		return;
	}

	wasKeyDown_ = true;

	SetState([this]()
	{
		std::string& value{ GetValue() };
		if (!value.empty())
		{
			value.pop_back();
		}
	});
}

void WValueBoxString::OnKeyBackspaceReleased()
{
	if (!isSelected_)
	{
		return;
	}

	wasKeyDown_ = false;
	isKeyHeld_ = false;
	keyHeldTime_ = 0.0f;
}

void WValueBoxString::OnKey(const Ekey key, const EInputState inputState)
{
	if (!isSelected_)
	{
		return;
	}

	const char character{ keyToChar(key) };
	if (isalpha(character))
	{
		SetState([this, character]()
		{
			GetValue().push_back(character);
		});
	}
}
