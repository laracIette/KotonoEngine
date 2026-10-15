#include "CoreWidgets/Button/Button.h"

#include <RenderGraph/InterfaceRenderGraph.h>

WButton::WButton()
	: isEnabled_{ true }
	, isPressed_{ false }
	, isActivated_{ false }
	, isActivatable_{ false }
	, startActivated_{ false }
{
	StyleClass = KButtonStyle::Class::Get();
}

void WButton::Init()
{
	Base::Init();
	
	isActivated_ = startActivated_;
	
	if (UObjectPtr const buttonStyle{ TryCast<KButtonStyle>(Style) })
	{
		disabledState_ = buttonStyle->GetDisabledState();
		normalState_ = buttonStyle->GetNormalState();
		focusedState_ = buttonStyle->GetFocusedState();
		pressedState_ = buttonStyle->GetPressedState();
		activatedNormalState_ = buttonStyle->GetActivatedNormalState();
		activatedFocusedState_ = buttonStyle->GetActivatedFocusedState();
		activatedPressedState_ = buttonStyle->GetActivatedPressedState();
	}
}

auto WButton::OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8
{
	if (!isEnabled_)
	{
		return INPUT_UNHANDLED;
	}

	if (button != EButton::Left)
	{
		return INPUT_UNHANDLED;
	}

	switch (inputState)
	{
	case EInputState::Pressed:
	{
		isPressed_ = true;
		pressedCursorPosition_ = cursorPosition_;

		if (onPressed_)
		{
			onPressed_();
		}

		if (isActivatable_)
		{
			isActivated_ = !isActivated_;

			if (isActivated_ && onActivated_)
			{
				onActivated_();
			}
			else if (onDeactivated_)
			{
				onDeactivated_();
			}
		}

		return INPUT_HANDLED;
	}
	case EInputState::Released:
	{
		if (!isPressed_)
		{
			break;
		}

		isPressed_ = false;

		if (onReleased_)
		{
			onReleased_();
		}

		if (!GetIsFocused())
		{
			break;
		}
		
		if (onClicked_)
		{
			onClicked_();
		}

		return INPUT_HANDLED;
	}
	default:
		break;
	}

	return INPUT_UNHANDLED;
}

auto WButton::OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) -> b8
{
	if (!isEnabled_)
	{
		return INPUT_UNHANDLED;
	}
	
	cursorPosition_ = position;

	if (!isPressed_)
	{
		return INPUT_UNHANDLED;
	}
	
	if (onDrag_)
	{
		onDrag_(delta, position - pressedCursorPosition_);
	}

	return INPUT_HANDLED;
}

void WButton::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph, UColor const& tint) const
{
	auto const [texture, color] { [this]() {
		b8 const isActivated{ isActivatable_ && isActivated_ };
		if (!GetIsEnabled())	return disabledState_;
		if (isPressed_)			return isActivated ? activatedPressedState_ : pressedState_;
		if (GetIsFocused())		return isActivated ? activatedFocusedState_ : focusedState_;
		return isActivated ? activatedNormalState_ : normalState_;
	}() };

	interfaceRenderGraph.drawDatas.push_back({
		.scissor{ GetScissor() },
		.modelMatrix{ GetModelMatrix() },
		.shader{ "${ENGINE_DIRECTORY}/Assets/shaders/shader2D.kasset" },
		.model{ "${ENGINE_DIRECTORY}/Assets/models/rectangle.obj" },
		.scalars{},
		.vectors{ color * tint * Tint },
		.textures{ texture },
		.isVisible{ GetIsVisible() },
	});
}

void WButton::Activate()
{
	Check(Warning, isActivatable_, "button is not activatable!");
	Check(Warning, isActivated_, "button is already activated!");
	Check(Warning, onActivated_, "onActivated_ is null!");
	
	if (!isActivatable_ || isActivated_)
	{
		return;
	}
	
	isActivated_ = true;
	
	if (onActivated_)
	{
		onActivated_();
	}
}

void WButton::Deactivate()
{
	Check(Warning, isActivatable_, "button is not activatable!");
	Check(Warning, !isActivated_, "button is already deactivated!");
	Check(Warning, onDeactivated_, "onDeactivated_ is null!");
	
	if (!isActivatable_ || !isActivated_)
	{
		return;
	}
	
	isActivated_ = false;
	
	if (onDeactivated_)
	{
		onDeactivated_();
	}
}

auto WButton::GetCanCache() const -> b8
{
	return Base::GetCanCache()
		&& isEnabled_.GetIsValue();
}

#include "Button.generated.inl"
