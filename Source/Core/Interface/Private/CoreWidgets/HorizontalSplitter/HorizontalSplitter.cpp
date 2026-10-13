#include "CoreWidgets/HorizontalSplitter/HorizontalSplitter.h"

#include "core_widgets.h"
#include "Interface/Interface.h"

WHorizontalSplitter::WHorizontalSplitter() 
	: startPreviousWidth_{ 0.0f }
	, startPreviousWeight_{ 0.0f }
{
	row_ = UCreate<WRow>{}();
}

WidgetPtr WHorizontalSplitter::Build()
{
	return (
		row_
		| Apply(&WRow::SetOnDisowned, [this](WidgetPtr const& widget) { RemoveChild(widget); })
	);
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
		Log(Warning, "child is null");
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
				| Apply(&WButton::SetOnPressed, [this, separator]() {
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
					
					previous_ = nullptr;
					next_ = nullptr;
					
					if (findPrevious != precedingView.end())
					{
						previous_ = *findPrevious;
						Check(Throw, previous_, "previous_ is null");
						startPreviousWidth_ = previous_->GetSize().x;
						startPreviousWeight_ = previous_->GetExpandWeight().x;
					}
					if (findNext != succeedingView.end())
					{
						next_ = *findNext;
						Check(Throw, next_, "next_ is null");
					}
				})
				| Apply(&WButton::SetOnDrag, [this](glm::vec2 const&, glm::vec2 const& total) {
					if (!previous_ || !next_)
					{
						Log(Warning, "previous_ or next_ is null");
						return;
					}
					
					auto const expandWeight{ previous_->GetExpandWeight().x + next_->GetExpandWeight().x };
					auto const combinedWidth{ previous_->GetSize().x + next_->GetSize().x };
					
					auto const newWidth{ std::clamp(startPreviousWidth_ + total.x, 1.0f, combinedWidth - 1.0f) };
					auto const newWeight{ newWidth / startPreviousWidth_ * startPreviousWeight_ };
					
					previous_->SetExpandWeight({ newWeight, 1.0f });
					next_->SetExpandWeight({ expandWeight - newWeight, 1.0f });
				})
			)
		);
		
		childSeparators_.TryEmplace(child, separator);
	}
	row_->AddChild(child);
}

void WHorizontalSplitter::RemoveChild(WidgetPtr const& child)
{
	if (!child)
	{
		Log(Warning, "child is null");
		return;
	}
	
	auto const it{ childSeparators_.Find(child) };
	if (childSeparators_.IsValidIterator(it))
	{
		UPtr const separator{ it->second };
		Check(Throw, separator, "separator is null");
		separator->Delete();
		
		childSeparators_.Remove(it);
	}
}

#include "HorizontalSplitter.generated.inl"
