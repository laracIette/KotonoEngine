#include "Detachable.h"

#include <Interface/Interface.h>
#include <core_widgets.h>

WDetachable::WDetachable()
{
	socket_ = UCreate<WSocket>{}();
}

WidgetPtr WDetachable::Build()
{
	return (
		UCreate<WStack>{}()
		| (
			UCreate<WColor>{}() 
			| Apply(&WColor::SetColor, Colors::White.WithValue(0.01f))
		)
		| (
			UCreate<WColumn>{}()
			| (
				UCreate<WConstraint>{}()
				| Apply(&WConstraint::SetAxis, EAxis::Vertical)
				| Apply(&WConstraint::SetSize, 20.0f)
				| (
					UCreate<WRow>{}()
					| (
						UCreate<WAlign>{}()
						| Apply(&WAlign::SetAlignment, UAlignment::Center())
						| (
							UCreate<WText>{ "Name" }()
							| Apply(&WText::SetText, [this]() { return GetName(); })
							| Apply(&WText::SetFontSize, glm::vec2{ 12.0f, 16.0f })
						)
					)
					| (
						UCreate<WBox>{}()
						| Apply(&WBox::SetSize, glm::vec2{ 20.0f, 20.0f })
						| (
							UCreate<WButton>{}()
							| Apply(&WButton::SetOnClicked, [this]() { Detach(); })
						)
					)
					| (
						UCreate<WBox>{}()
						| Apply(&WBox::SetSize, glm::vec2{ 20.0f, 20.0f })
						| (
							UCreate<WButton>{}()
							| Apply(&WButton::SetOnClicked, [this]() { Delete(); })
							| Apply(&WButton::SetNormalColor, Colors::Red)
						)
					)
				)
			)
			| (
				socket_
			)
		)
	);
}

auto WDetachable::GetChild() const -> WidgetPtr
{
	return socket_ ? socket_->GetChild() : nullptr;
}

void WDetachable::SetChild(WidgetPtr const& widget) const
{
	UPtr const child{ socket_->GetChild() };

	socket_->SetChild(widget);

	if (child)
	{
		child->Delete();
	}
}

void WDetachable::Detach()
{
	UPtr const child{ GetChild() };

	if (!child)
	{
		return;
	}
	
	socket_->SetChild(nullptr);

	GetInterface()->OpenWidgetInWindow(child->GetSize(), child, GetName());

	Delete();
}

#include "Detachable.generated.inl"
