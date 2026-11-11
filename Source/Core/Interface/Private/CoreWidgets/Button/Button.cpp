#include "CoreWidgets/Button/Button.h"

#include <RenderGraph/InterfaceRenderGraph.h>

WButton::WButton()
	: isEnabled_{ true }
	, isPressed_{ false }
	, isActivated_{ false }
	, isActivatable_{ false }
	, startActivated_{ false }
	, disabledState_{}
	, normalState_{}
	, focusedState_{}
	, pressedState_{}
	, activatedNormalState_{}
	, activatedFocusedState_{}
	, activatedPressedState_{}
	, hasShadow_{ false }
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
		hasShadow_ = buttonStyle->GetHasShadow();
		shadowOffset_ = buttonStyle->GetShadowOffset();
		shadowColor_ = buttonStyle->GetShadowColor();
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
			onPressed_(modifier);
		}

		if (isActivatable_)
		{
			isActivated_ = !isActivated_;

			if (isActivated_)
			{
			    if (onActivated_)
			    {
				    onActivated_();
			    }
			}
			else
			{
			    if (onDeactivated_)
			    {
			        onDeactivated_();
			    }
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
			onReleased_(modifier);
		}

		if (!GetIsFocused())
		{
			break;
		}
		
		if (onClicked_)
		{
			onClicked_(modifier);
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
	auto const& state { [this]() -> KButtonStyle::State const& {
		if (!GetIsEnabled())	return disabledState_;
		if (isPressed_)			return isActivated_ ? activatedPressedState_ : pressedState_;
		if (GetIsFocused())		return isActivated_ ? activatedFocusedState_ : focusedState_;
		return isActivated_ ? activatedNormalState_ : normalState_;
	}() };
	
	if (hasShadow_)
	{
		PopulateRenderGraphShadow(interfaceRenderGraph, tint, state);
	}
	PopulateRenderGraphImage(interfaceRenderGraph, tint, state);
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

void WButton::PopulateRenderGraphShadow(UInterfaceRenderGraph& interfaceRenderGraph, UColor const& tint, KButtonStyle::State const& state) const
{
	switch (state.imageType)
	{
	case KButtonStyle::EImageType::None:
	{
		break;
	}
	case KButtonStyle::EImageType::Normal:
	{
		interfaceRenderGraph.drawDatas.push_back(
			UInterfaceDrawData::MakeImage(
				GetScissor(),
				GetModelMatrix(),
				shadowColor_ * tint * Tint,
				UPath{ "${ENGINE_DIRECTORY}/Assets/textures/white_texture.jpg" },
				GetIsVisible()
			)
		);
		break;
	}
	case KButtonStyle::EImageType::Rounded:
	{
		interfaceRenderGraph.drawDatas.push_back(
			UInterfaceDrawData::MakeRoundedImage(
				GetScissor(),
				GetModelMatrix(),
				shadowColor_ * tint * Tint,
				UPath{ "${ENGINE_DIRECTORY}/Assets/textures/white_texture.jpg" },
				state.corners,
				GetPosition(),
				GetSize(),
				GetIsVisible()
			)
		);
		break;
	}
	}
}

void WButton::PopulateRenderGraphImage(UInterfaceRenderGraph& interfaceRenderGraph, UColor const& tint, KButtonStyle::State const& state) const
{
	glm::mat4 modelMatrix;
	glm::vec2 position;
	
	if (hasShadow_ && !isPressed_)
	{
		modelMatrix = GetModelMatrix(GetPosition() + shadowOffset_, 0.0f, GetSize());
		position = GetPosition() + shadowOffset_;
	}
	else
	{
		modelMatrix = GetModelMatrix();
		position = GetPosition();
	}
	
	switch (state.imageType)
	{
	case KButtonStyle::EImageType::None:
	{
		break;
	}
	case KButtonStyle::EImageType::Normal:
	{
		interfaceRenderGraph.drawDatas.push_back(
			UInterfaceDrawData::MakeImage(
				GetScissor(),
				modelMatrix,
				state.color * tint * Tint,
				state.texture,
				GetIsVisible()
			)
		);
		break;
	}
	case KButtonStyle::EImageType::Rounded:
	{
		interfaceRenderGraph.drawDatas.push_back(
			UInterfaceDrawData::MakeRoundedImage(
				GetScissor(),
				modelMatrix,
				state.color * tint * Tint,
				state.texture,
				state.corners,
				position,
				GetSize(),
				GetIsVisible()
			)
		);
		break;
	}
	}
}

#include "Button.generated.inl"
