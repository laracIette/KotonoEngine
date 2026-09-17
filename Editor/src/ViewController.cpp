#include "ViewController.h"

#include <glm/ext/quaternion_trigonometric.hpp>
#include <kotono_core/Interface.h>
#include <kotono_core/Scene.h>
#include <kotono_input/Keyboard.h>
#include <kotono_input/Mouse.h>
#include <kotono_interface/widgets.h>
#include <kotono_math/math_utils.h>

WViewController::WViewController()
	: WViewController(nullptr)
{
}

WViewController::WViewController(UPtr<WSceneContext> const& sceneContext)
	: Base(sceneContext)
	, isActive_{ false }
	, speed_{ 1.0f }
	, sensitivity_{ 0.005f }
	, pitch_{ 0.0f }
	, yaw_{ glm::radians(180.0f) }
{
}

WidgetPtr WViewController::Build()
{
    UPtr button{ UCreate<WButton>{ "Button" }() };
    button->SetIsVisible(false);
	button->SetOnActive([this]() { isActive_ = true; });
	button->SetOnInactive([this]() { isActive_ = false; });

	auto const widgetTree{ UChildrenOwnerTree{ UCreate<WStack>{}(), {
		new UWidgetTreeLeaf{ sceneTexture_ = UCreate<WSceneTexture>{ "Scene Texture" }() },
		new UWidgetTreeLeaf{ button },
	} } };
	widgetTree.Link();

    return widgetTree.Widget();
}

void WViewController::Display(UWidgetDisplaySettings const& displaySettings)
{
    Base::Display(displaySettings);

    Keyboard.GetEventKey(EKey::W, EInputState::Down).AddListener(this, &Self::OnKeyboardWKeyDown);
    Keyboard.GetEventKey(EKey::A, EInputState::Down).AddListener(this, &Self::OnKeyboardAKeyDown);
    Keyboard.GetEventKey(EKey::S, EInputState::Down).AddListener(this, &Self::OnKeyboardSKeyDown);
    Keyboard.GetEventKey(EKey::D, EInputState::Down).AddListener(this, &Self::OnKeyboardDKeyDown);
    Keyboard.GetEventKey(EKey::Q, EInputState::Down).AddListener(this, &Self::OnKeyboardQKeyDown);
    Keyboard.GetEventKey(EKey::E, EInputState::Down).AddListener(this, &Self::OnKeyboardEKeyDown);
    Mouse.GetEventVerticalScroll().AddListener(this, &Self::OnMouseVerticalScroll);
}

void WViewController::Remove()
{
    Base::Remove();

    Keyboard.GetEventKey(EKey::W, EInputState::Down).RemoveListener(this, &Self::OnKeyboardWKeyDown);
    Keyboard.GetEventKey(EKey::A, EInputState::Down).RemoveListener(this, &Self::OnKeyboardAKeyDown);
    Keyboard.GetEventKey(EKey::S, EInputState::Down).RemoveListener(this, &Self::OnKeyboardSKeyDown);
    Keyboard.GetEventKey(EKey::D, EInputState::Down).RemoveListener(this, &Self::OnKeyboardDKeyDown);
    Keyboard.GetEventKey(EKey::Q, EInputState::Down).RemoveListener(this, &Self::OnKeyboardQKeyDown);
    Keyboard.GetEventKey(EKey::E, EInputState::Down).RemoveListener(this, &Self::OnKeyboardEKeyDown);
    Mouse.GetEventVerticalScroll().RemoveListener(this, &Self::OnMouseVerticalScroll);
}

b8 WViewController::OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position)
{
	if (!isActive_)
	{
		return INPUT_UNHANDLED;
	}

	pitch_ -= delta.y * sensitivity_;
	yaw_ += delta.x * sensitivity_;

	// Clamp pitch to avoid flipping
	pitch_ = glm::clamp(pitch_, -glm::half_pi<f32>(), glm::half_pi<f32>());

	glm::quat const qPitch{ glm::angleAxis(pitch_, WorldRightVector) };
	glm::quat const qYaw{ glm::angleAxis(yaw_, WorldUpVector) };

	if (sceneTexture_)
	{
		glm::quat const rotation{ qYaw * qPitch };
		sceneTexture_->SetViewRotation(rotation); 
		GetScene()->GetAudioContext().SetListenerOrientation(rotation);
	}

	return INPUT_HANDLED;
}

void WViewController::OnKeyboardWKeyDown() const
{
	if (sceneTexture_)
	{
		auto const direction{ sceneTexture_->GetForwardVector() };
		Translate(direction * GetInterface()->GetDeltaTime() * speed_);
	}
}

void WViewController::OnKeyboardAKeyDown() const
{
	if (sceneTexture_)
	{
		auto const direction{ sceneTexture_->GetRightVector() };
		Translate(direction * GetInterface()->GetDeltaTime() * speed_);
	}
}

void WViewController::OnKeyboardSKeyDown() const
{
	if (sceneTexture_)
	{
		auto const direction{ -sceneTexture_->GetForwardVector() };
		Translate(direction * GetInterface()->GetDeltaTime() * speed_);
	}
}

void WViewController::OnKeyboardDKeyDown() const
{
	if (sceneTexture_)
	{
		auto const direction{ -sceneTexture_->GetRightVector() };
		Translate(direction * GetInterface()->GetDeltaTime() * speed_);
	}
}

void WViewController::OnKeyboardQKeyDown() const
{
	Translate(-WorldUpVector * GetInterface()->GetDeltaTime() * speed_);
}

void WViewController::OnKeyboardEKeyDown() const
{
	Translate(WorldUpVector * GetInterface()->GetDeltaTime() * speed_);
}

void WViewController::OnMouseVerticalScroll(f32 delta)
{
	if (!isActive_)
	{
		return;
	}

	speed_ += speed_ * delta / 10.0f;
	speed_ = std::clamp(speed_, 0.1f, 100.0f);
}

void WViewController::Translate(glm::vec3 const& delta) const
{
	if (!isActive_)
	{
		return;
	}

	if (sceneTexture_)
	{
		auto const position{ sceneTexture_->GetViewPosition() + delta };
		sceneTexture_->SetViewPosition(position);
		GetScene()->GetAudioContext().SetListenerPosition(position);
	}
}

#include "generated/ViewController.generated.inl"