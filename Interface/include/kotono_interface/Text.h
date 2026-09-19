#pragma once
#include "generated/Text.generated.h"
#include <kotono_core/Widget.h>

#include <kotono_common/Bindable.h>
/// Display a text
class WText final : public WWidget
{
	GENERATED_WTEXT()

private:
	struct CharacterData
	{
		UPath path;
		glm::mat4 modelMatrix;
	};

public:
	WText(std::string_view text = "", glm::vec2 const& fontSize = { 20.0f, 24.0f }, f32 spacing = 0.75f);

	glm::vec2 GetContentSize(glm::vec2 bounds) const override;
	glm::vec2 GetDesiredSize(glm::vec2 const& bounds) const override;

	EFlex GetFlex() const override;
	EExpand GetExpand() const override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

	auto GetText() const -> std::string;
	void SetText(UBindable<std::string> const& text);

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	UBindable<std::string> text_;
	StateProperty(glm::vec2, fontSize_, FontSize, Value);
	StateProperty(f32, spacing_, Spacing, Value);

	std::vector<CharacterData> characters_;
};

