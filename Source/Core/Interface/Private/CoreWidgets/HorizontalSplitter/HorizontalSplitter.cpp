#include "CoreWidgets/HorizontalSplitter/HorizontalSplitter.h"

#include "core_widgets.h"

WHorizontalSplitter::WHorizontalSplitter()
{
	row_ = UCreate<WRow>{}();
}

WidgetPtr WHorizontalSplitter::Build()
{
	return row_;
}

void WHorizontalSplitter::SetChildren(WidgetSet const& children)
{
	for (auto const& child : children)
	{
		AddChild(child);
	}
}

void WHorizontalSplitter::AddChild(WidgetPtr const& child)
{
	if (!child)
	{
		Log(Warning, "Interface", "child is null");
		return;
	}
	
	if (!row_->GetChildren().empty())
	{
		UPtr const separator{ UCreate<WConstraint>{}() };
		row_->AddChild(
			separator
			| Apply(&WConstraint::SetAxis, EAxis::Horizontal)
			| Apply(&WConstraint::SetSize, 10.0f)
			| (
				UCreate<WButton>{}()
				| Apply(&WButton::SetOnDrag, [this, separator](glm::vec2 const& delta) {
					auto const index{ row_->GetChildren().GetIndex(separator) };
					
					auto const precedingView{ row_->GetChildren() | std::views::take(index) | std::views::reverse };
					auto const findPrevious{ std::ranges::find_if(precedingView
						, [](WidgetPtr const& widget) {
							return widget->GetHasExpand(EExpand::Horizontal);
						}
					) };
					
					auto const succeedingView{ row_->GetChildren() | std::views::drop(index + 1) };
					auto const findNext{ std::ranges::find_if(succeedingView
						, [](WidgetPtr const& widget) {
							return widget->GetHasExpand(EExpand::Horizontal);
						}
					) };
					
					if (findPrevious == precedingView.end() || findNext == succeedingView.end())
					{
						return;
					}
					
					UPtr const previous{ *findPrevious };
					UPtr const next{ *findNext };
					
					auto const expandWeight{ previous->GetExpandWeight().x + next->GetExpandWeight().x };
					
					const auto width{ previous->GetSize().x };
					const auto weight{ previous->GetExpandWeight().x };
					const auto result{ std::clamp(width / (width - delta.x) * weight, 0.01f, expandWeight - 0.01f) };
					
					previous->SetExpandWeight(glm::vec2{ result, 1.0f });
					next->SetExpandWeight(glm::vec2{ expandWeight - result, 1.0f });
				})
			)
		);
	}
	row_->AddChild(child);
}

#include "HorizontalSplitter.generated.inl"
