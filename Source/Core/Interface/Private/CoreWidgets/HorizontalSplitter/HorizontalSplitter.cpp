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
		row_->AddChild(
			UCreate<WConstraint>{}()
			| Apply(&WConstraint::SetAxis, EAxis::Horizontal)
			| Apply(&WConstraint::SetSize, 10.0f)
			| (
				UCreate<WButton>{}()
				| Apply(&WButton::SetOnDrag, [previous = row_->GetChildren().back(), next = child](glm::vec2 const& delta) {
					Check(Throw, previous, "previous child is null");
					Check(Throw, next, "next child is null");
					
					auto const flexCount{ std::ranges::count_if(std::array{ previous, next }
					  , [](WidgetPtr const& widget) {
						  return widget->GetHasFlex(EFlex::Horizontal);
					  }
					) };
					
					if (flexCount == 0)
					{
						return;
					}
					
					auto const perWidgetAmount{ delta.x / flexCount / 500.0f };
					
					if (previous->GetHasFlex(EFlex::Horizontal))
					{
						previous->SetExpandWeight(previous->GetExpandWeight() + glm::vec2{ perWidgetAmount, 0.0f });
					}
					if (next->GetHasFlex(EFlex::Horizontal))
					{
						next->SetExpandWeight(next->GetExpandWeight() - glm::vec2{ perWidgetAmount, 0.0f });
					}
				})
			)
		);
	}
	row_->AddChild(child);
}

#include "HorizontalSplitter.generated.inl"
