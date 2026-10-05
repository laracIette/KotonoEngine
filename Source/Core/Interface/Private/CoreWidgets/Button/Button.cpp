#include "CoreWidgets/Button/Button.h"

#include <RenderGraph/InterfaceRenderGraph.h>

static const UPath DEFAULT_TEXTURE{ "${ENGINE_DIRECTORY}/Assets/textures/white_texture.jpg" };

static constexpr UColor DEFAULT_NORMAL{ Colors::White.WithValue(0.1f) };
static constexpr UColor DEFAULT_FOCUSED{ Colors::White.WithValue(0.15f) };
static constexpr UColor DEFAULT_PRESSED{ Colors::White.WithValue(0.25f) };
static constexpr UColor DEFAULT_ACTIVATED{ Colors::White.WithValue(0.2f) };
static constexpr UColor DEFAULT_DISABLED{ DEFAULT_NORMAL.WithAlpha(0.5f) };

WButton::WButton()
	: isEnabled_{ true }
	, isPressed_{ false }
	, isActivated_{ false }
	, isActivatable_{ false }
	, normalState_{ DEFAULT_TEXTURE, DEFAULT_NORMAL }
	, focusedState_{ DEFAULT_TEXTURE, DEFAULT_FOCUSED }
	, pressedState_{ DEFAULT_TEXTURE, DEFAULT_PRESSED }
	, activatedState_{ DEFAULT_TEXTURE, DEFAULT_ACTIVATED }
	, disabledState_{ DEFAULT_TEXTURE, DEFAULT_DISABLED }
{
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

	if (!isPressed_)
	{
		return INPUT_UNHANDLED;
	}
	
	if (onDrag_)
	{
		onDrag_(delta);
	}

	return INPUT_HANDLED;
}

void WButton::OnUnfocused()
{
	Base::OnUnfocused();

	if (isPressed_)
	{
		isPressed_ = false;

		if (onReleased_)
		{
			onReleased_();
		}
	}
}

void WButton::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	auto const state{ [this]() {
		if (!GetIsEnabled())	return disabledState_;
		if (isPressed_)			return pressedState_;
		if (GetIsFocused())		return focusedState_;
		if (isActivated_)		return activatedState_;
		return normalState_;
	}() };

	interfaceRenderGraph.drawDatas.push_back({
		.scissor = GetScissor(),
		.modelMatrix = GetModelMatrix(),
		.shader = "${ENGINE_DIRECTORY}/Assets/shaders/shader2D.kasset",
		.model = "${ENGINE_DIRECTORY}/Assets/models/rectangle.obj",
		.scalars = {},
		.vectors = { state.color },
		.textures = { state.texture },
		.isVisible = GetIsVisible(),
	});
}

auto WButton::GetCanCache() const -> b8
{
	return Base::GetCanCache()
		&& isEnabled_.GetIsValue();
}

#include "Button.generated.inl"
