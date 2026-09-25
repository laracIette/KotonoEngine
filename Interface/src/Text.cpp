#include "Text.h"

#include <glm/ext/matrix_transform.hpp>
#include <kotono_core/Interface.h>
#include <kotono_graphics/Color.h>
#include <kotono_graphics/Font.h>
#include <kotono_graphics/InterfaceRenderGraph.h>
#include <kotono_math/math_utils.h>

WText::WText()
	: text_{}
	, fontSize_{ 20.0f, 24.0f }
	, spacing_{ 0.75f }
{
}

auto WText::GetContentSize(glm::vec2 const& bounds) const -> glm::vec2
{
	return glm::min(GetDesiredSize(bounds), bounds);
}

auto WText::GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2
{
	auto const length{ GetText().length() };
	if (length == 0)
	{
		return { 0.0f, fontSize_.y };
	}

	glm::vec2 const size{
		fontSize_.x * (1.0f + spacing_ * (length - 1)),
		fontSize_.y,
	};

	return size;
}

auto WText::GetFlex() const -> EFlex
{
	return EFlex::None;
}

auto WText::GetExpand() const -> EExpand
{
	return EExpand::None;
}

void WText::PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const
{
	for (auto const& [characterPath, modelMatrix] : characters_)
	{
		interfaceRenderGraph.drawDatas.push_back({
			.scissor = GetScissor(),
			.modelMatrix = modelMatrix,
			.shader = "${ENGINE_DIRECTORY}/Graphics/assets/shaders/shader2D.kasset",
			.model = "${ENGINE_DIRECTORY}/Graphics/assets/models/rectangle.obj",
			.scalars = {},
			.vectors = { Colors::White },
			.textures = { characterPath },
			.isVisible = GetIsVisible(),
		});
	}
}

auto WText::GetText() const -> std::string
{
	return text_;
}

void WText::SetText(UBindable<std::string> const& text)
{
	SetState([this, text]() { text_ = text; });
}

void WText::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	characters_.clear();

	static UFont const font{ "${ENGINE_DIRECTORY}/Graphics/assets/fonts/default" };
	auto const characterPaths{ font.GetTextPaths(GetText()) };

	for (auto const& [index, characterPath] : characterPaths | std::views::enumerate)
	{
		if (fontSize_.x * spacing_ * (index + 1) > displaySettings.bounds.x)
		{
			break;
		}

		glm::vec2 const offset{
			fontSize_.x * 0.5f + fontSize_.x * spacing_ * index,
			fontSize_.y * 0.5f
		};

		auto const position{ GetPosition() + offset };

		glm::vec2 const bounds{ GetInterface()->GetBounds() };
		auto const modelMatrix{
			glm::translate(glm::identity<glm::mat4>(), { px_to_ndc_pos(position, bounds), 0.0f })
		  * glm::scale(glm::identity<glm::mat4>(), { px_to_ndc_size(fontSize_, bounds), 1.0f })
		};

		characters_.emplace_back(characterPath, modelMatrix);
	}
}

auto WText::GetCanCache() const -> b8
{
	return text_.GetIsValue();
}

#include "generated/Text.generated.inl"
