#include "Button.h"

#include <kotono_graphics/InterfaceRenderGraph.h>

static constexpr UColor DEFAULT_NORMAL{ Colors::White.WithValue(0.1f) };
static constexpr UColor DEFAULT_FOCUSED{ Colors::White.WithValue(0.15f) };
static constexpr UColor DEFAULT_PRESSED{ Colors::White.WithValue(0.25f) };
static constexpr UColor DEFAULT_SELECTED{ Colors::White.WithValue(0.2f) };
static constexpr UColor DEFAULT_DISABLED{ DEFAULT_NORMAL.WithAlpha(0.5f) };

WButton::WButton()
	: isPressed_{ false }
	, isSelected_{ false }
	, isEnabled_{ true }
	, isSelectable_{ false }
	, normalColor_{ DEFAULT_NORMAL }
	, focusedColor_{ DEFAULT_FOCUSED }
	, pressedColor_{ DEFAULT_PRESSED }
	, selectedColor_{ DEFAULT_SELECTED }
	, disabledColor_{ DEFAULT_DISABLED }
{
}

b8 WButton::OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position)
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

		if (onActive_)
		{
			onActive_();
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

		if (onInactive_)
		{
			onInactive_();
		}

		if (onClicked_)
		{
			onClicked_();
		}

		if (isSelectable_ && !isSelected_)
		{
			isSelected_ = true;
			if (onSelected_)
			{
				onSelected_();
			}
		}

		return INPUT_HANDLED;
	}
	case EInputState::Down:
	{
		if (!isEnabled_)
		{
			break;
		}

		if (!isPressed_)
		{
			break;
		}

		if (onDown_)
		{
			onDown_();
		}

		return INPUT_HANDLED;
	}
	default:
		break;
	}

	return INPUT_UNHANDLED;
}

b8 WButton::OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position)
{
	return INPUT_UNHANDLED;
}

void WButton::OnUnfocused()
{
	Base::OnUnfocused();

	if (isPressed_)
	{
		isPressed_ = false;

		if (onInactive_)
		{
			onInactive_();
		}
	}
}

void WButton::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	auto const getColor{ [this]() {
		if (!GetIsEnabled())	return disabledColor_;
		if (isPressed_)			return pressedColor_;
		if (isSelected_)		return selectedColor_;
		if (GetIsFocused())		return focusedColor_;
		return GetNormalColor();
	} };

	interfaceRenderGraph.drawDatas.push_back({
		.scissor = GetScissor(),
		.modelMatrix = GetModelMatrix(),
		.shader = "${ENGINE_DIRECTORY}/Graphics/assets/shaders/shader2D.kasset",
		.model = "${ENGINE_DIRECTORY}/Graphics/assets/models/rectangle.obj",
		.scalars = {},
		.vectors = { getColor() },
		.textures = { "${ENGINE_DIRECTORY}/Graphics/assets/textures/white_texture.jpg" },
		.isVisible = GetIsVisible(),
	});
}

auto WButton::GetIsEnabled() const -> b8
{
	return isEnabled_;
}

auto WButton::GetNormalColor() const -> UColor
{
	return normalColor_;
}

void WButton::SetIsEnabled(UBindable<b8> const& isEnabled)
{
	isEnabled_ = isEnabled;
}

void WButton::SetNormalColor(UBindable<UColor> const& color)
{
	normalColor_ = color;
}

auto WButton::GetCanCache() const -> b8
{
	return isEnabled_.GetIsValue()
		&& normalColor_.GetIsValue();
}

#include "generated/Button.generated.inl"
