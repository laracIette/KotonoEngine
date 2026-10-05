#pragma once
#include "Text.generated.h"
#include <Widget/Widget.h>

#include <Font/Font.h>

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
	WText();

	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;
	auto GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2 override;

	auto GetFlex() const -> EFlex override;
	auto GetExpand() const -> EExpand override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

	auto GetText() const -> std::string;
	void SetText(UBindable<std::string> const& text);

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

	auto GetCanCache() const -> b8 override;

private:
	UBindable<std::string> text_;
	StateProperty(glm::vec2, fontSize_, FontSize, Value);
	StateProperty(f32, spacing_, Spacing, Value);
	StateProperty(UFont, font_, Font);

	std::vector<CharacterData> characters_;
};

