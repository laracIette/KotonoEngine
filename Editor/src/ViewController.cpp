#include "ViewController.h"
#include <kotono_core/TimeManager.h>
#include <kotono_graphics/RenderContext.h>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#include <kotono_interface/widgets.h>
#include <kotono_platform/glm_utils.h>

WViewController::WViewController()
	: isFocused_{ false }
	, speed_{ 1.0f }
	, sensitivity_{ 0.005f }
	, pitch_{ 0.0f }
	, yaw_{ glm::radians(180.0f) }
{
}

WidgetPtr WViewController::Build()
{
    UPtr button{ UCreate<WButton>{ "Button" }() };
    button->SetOnActive([this]() { isFocused_ = true; });
    button->SetOnInactive([this]() { isFocused_ = false; });

    return button;
}

void WViewController::Display(UWidgetDisplaySettings displaySettings)
{
    Base::Display(displaySettings);

    Keyboard.EventKey(EKey::W, EInputState::Down).AddListener(this, &WViewController::OnKeyboardWKeyDown);
    Keyboard.EventKey(EKey::A, EInputState::Down).AddListener(this, &WViewController::OnKeyboardAKeyDown);
    Keyboard.EventKey(EKey::S, EInputState::Down).AddListener(this, &WViewController::OnKeyboardSKeyDown);
    Keyboard.EventKey(EKey::D, EInputState::Down).AddListener(this, &WViewController::OnKeyboardDKeyDown);
    Keyboard.EventKey(EKey::Q, EInputState::Down).AddListener(this, &WViewController::OnKeyboardQKeyDown);
    Keyboard.EventKey(EKey::E, EInputState::Down).AddListener(this, &WViewController::OnKeyboardEKeyDown);
    Mouse.EventMove().AddListener(this, &WViewController::OnMouseMove);
    Mouse.EventVerticalScroll().AddListener(this, &WViewController::OnMouseVerticalScroll);
}

void WViewController::Remove()
{
    Base::Remove();

    Keyboard.EventKey(EKey::W, EInputState::Down).RemoveListener(this, &WViewController::OnKeyboardWKeyDown);
    Keyboard.EventKey(EKey::A, EInputState::Down).RemoveListener(this, &WViewController::OnKeyboardAKeyDown);
    Keyboard.EventKey(EKey::S, EInputState::Down).RemoveListener(this, &WViewController::OnKeyboardSKeyDown);
    Keyboard.EventKey(EKey::D, EInputState::Down).RemoveListener(this, &WViewController::OnKeyboardDKeyDown);
    Keyboard.EventKey(EKey::Q, EInputState::Down).RemoveListener(this, &WViewController::OnKeyboardQKeyDown);
    Keyboard.EventKey(EKey::E, EInputState::Down).RemoveListener(this, &WViewController::OnKeyboardEKeyDown);
    Mouse.EventMove().RemoveListener(this, &WViewController::OnMouseMove);
    Mouse.EventVerticalScroll().RemoveListener(this, &WViewController::OnMouseVerticalScroll);
}


void WViewController::OnKeyboardWKeyDown() const
{
	const auto direction{ GetRenderContext()->GetViewPoint().GetForwardVector() };
	Translate(direction * TimeManager.Delta() * speed_);
}

void WViewController::OnKeyboardAKeyDown() const
{
	const auto direction{ GetRenderContext()->GetViewPoint().GetRightVector() };
	Translate(direction * TimeManager.Delta() * speed_);
}

void WViewController::OnKeyboardSKeyDown() const
{
	const auto direction{ -GetRenderContext()->GetViewPoint().GetForwardVector() };
	Translate(direction * TimeManager.Delta() * speed_);
}

void WViewController::OnKeyboardDKeyDown() const
{
	const auto direction{ -GetRenderContext()->GetViewPoint().GetRightVector() };
	Translate(direction * TimeManager.Delta() * speed_);
}

void WViewController::OnKeyboardQKeyDown() const
{
	Translate(-WorldUpVector * TimeManager.Delta() * speed_);
}

void WViewController::OnKeyboardEKeyDown() const
{
	Translate(WorldUpVector * TimeManager.Delta() * speed_);
}

void WViewController::OnMouseMove(const glm::vec2& delta)
{
	if (!isFocused_)
	{
		return;
	}

	pitch_ -= delta.y * sensitivity_;
	yaw_ += delta.x * sensitivity_;

	// Clamp pitch to avoid flipping
	pitch_ = glm::clamp(pitch_, -glm::half_pi<f32>(), glm::half_pi<f32>());

	const glm::quat qPitch{ glm::angleAxis(pitch_, WorldRightVector) };
	const glm::quat qYaw{ glm::angleAxis(yaw_, WorldUpVector) };

	const glm::quat rotation{ qYaw * qPitch };
	GetRenderContext()->GetViewPoint().SetRotation(rotation);
}

void WViewController::OnMouseVerticalScroll(const f32 delta)
{
	if (!isFocused_)
	{
		return;
	}

	speed_ += speed_ * delta / 10.0f;
	speed_ = std::clamp(speed_, 0.1f, 100.0f);
}

void WViewController::Translate(const glm::vec3& delta) const
{
	if (!isFocused_)
	{
		return;
	}

	const auto& position{ GetRenderContext()->GetViewPoint().GetPosition() };
	GetRenderContext()->GetViewPoint().SetPosition(position + delta);
}

#include "generated/ViewController.generated.inl"