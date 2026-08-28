#include "ViewController.h"

#include <glm/ext/quaternion_trigonometric.hpp>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#include <kotono_interface/widgets.h>
#include <kotono_object/Interface.h>
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

	auto const widgetTree{ UChildrenOwnerTree{ UCreate<WStack>{}(), {
		new UWidgetTreeLeaf{ sceneRenderer_ = UCreate<WSceneRenderer>{ "Scene Renderer" }() },
		new UWidgetTreeLeaf{ button },
	} } };
	widgetTree.Link();

    return widgetTree.Widget();
}

void WViewController::Display(UWidgetDisplaySettings const& displaySettings)
{
    Base::Display(displaySettings);

    Keyboard.GetEventKey(EKey::W, EInputState::Down).AddListener(this, &WViewController::OnKeyboardWKeyDown);
    Keyboard.GetEventKey(EKey::A, EInputState::Down).AddListener(this, &WViewController::OnKeyboardAKeyDown);
    Keyboard.GetEventKey(EKey::S, EInputState::Down).AddListener(this, &WViewController::OnKeyboardSKeyDown);
    Keyboard.GetEventKey(EKey::D, EInputState::Down).AddListener(this, &WViewController::OnKeyboardDKeyDown);
    Keyboard.GetEventKey(EKey::Q, EInputState::Down).AddListener(this, &WViewController::OnKeyboardQKeyDown);
    Keyboard.GetEventKey(EKey::E, EInputState::Down).AddListener(this, &WViewController::OnKeyboardEKeyDown);
    Mouse.GetEventMove().AddListener(this, &WViewController::OnMouseMove);
    Mouse.GetEventVerticalScroll().AddListener(this, &WViewController::OnMouseVerticalScroll);
}

void WViewController::Remove()
{
    Base::Remove();

    Keyboard.GetEventKey(EKey::W, EInputState::Down).RemoveListener(this, &WViewController::OnKeyboardWKeyDown);
    Keyboard.GetEventKey(EKey::A, EInputState::Down).RemoveListener(this, &WViewController::OnKeyboardAKeyDown);
    Keyboard.GetEventKey(EKey::S, EInputState::Down).RemoveListener(this, &WViewController::OnKeyboardSKeyDown);
    Keyboard.GetEventKey(EKey::D, EInputState::Down).RemoveListener(this, &WViewController::OnKeyboardDKeyDown);
    Keyboard.GetEventKey(EKey::Q, EInputState::Down).RemoveListener(this, &WViewController::OnKeyboardQKeyDown);
    Keyboard.GetEventKey(EKey::E, EInputState::Down).RemoveListener(this, &WViewController::OnKeyboardEKeyDown);
    Mouse.GetEventMove().RemoveListener(this, &WViewController::OnMouseMove);
    Mouse.GetEventVerticalScroll().RemoveListener(this, &WViewController::OnMouseVerticalScroll);
}


void WViewController::OnKeyboardWKeyDown() const
{
	if (sceneRenderer_)
	{
		auto const direction{ sceneRenderer_->GetForwardVector() };
		Translate(direction * GetInterface()->GetTimeContext().lastDelta * speed_);
	}
}

void WViewController::OnKeyboardAKeyDown() const
{
	if (sceneRenderer_)
	{
		auto const direction{ sceneRenderer_->GetRightVector() };
		Translate(direction * GetInterface()->GetTimeContext().lastDelta * speed_);
	}
}

void WViewController::OnKeyboardSKeyDown() const
{
	if (sceneRenderer_)
	{
		auto const direction{ -sceneRenderer_->GetForwardVector() };
		Translate(direction * GetInterface()->GetTimeContext().lastDelta * speed_);
	}
}

void WViewController::OnKeyboardDKeyDown() const
{
	if (sceneRenderer_)
	{
		auto const direction{ -sceneRenderer_->GetRightVector() };
		Translate(direction * GetInterface()->GetTimeContext().lastDelta * speed_);
	}
}

void WViewController::OnKeyboardQKeyDown() const
{
	Translate(-WorldUpVector * GetInterface()->GetTimeContext().lastDelta * speed_);
}

void WViewController::OnKeyboardEKeyDown() const
{
	Translate(WorldUpVector * GetInterface()->GetTimeContext().lastDelta * speed_);
}

void WViewController::OnMouseMove(glm::vec2 const& delta)
{
	if (!isFocused_)
	{
		return;
	}

	pitch_ -= delta.y * sensitivity_;
	yaw_ += delta.x * sensitivity_;

	// Clamp pitch to avoid flipping
	pitch_ = glm::clamp(pitch_, -glm::half_pi<f32>(), glm::half_pi<f32>());

	glm::quat const qPitch{ glm::angleAxis(pitch_, WorldRightVector) };
	glm::quat const qYaw{ glm::angleAxis(yaw_, WorldUpVector) };
	
	if (sceneRenderer_)
	{
		glm::quat const rotation{ qYaw * qPitch };
		sceneRenderer_->SetViewRotation(rotation);
	}
}

void WViewController::OnMouseVerticalScroll(f32 delta)
{
	if (!isFocused_)
	{
		return;
	}

	speed_ += speed_ * delta / 10.0f;
	speed_ = std::clamp(speed_, 0.1f, 100.0f);
}

void WViewController::Translate(glm::vec3 const& delta) const
{
	if (!isFocused_)
	{
		return;
	}

	if (sceneRenderer_)
	{
		auto const& position{ sceneRenderer_->GetViewPosition() };
		sceneRenderer_->SetViewPosition(position + delta);
	}
}

#include "generated/ViewController.generated.inl"