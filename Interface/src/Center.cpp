#include "Center.h"

WCenter::WCenter(const EAxis axis)
	: axis_{ axis }
{
}

glm::vec2 WCenter::GetContentSize(glm::vec2 bounds) const
{
	return Base::GetContentSize(bounds);
}

void WCenter::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	if (GetChild())
	{
		const auto childSize{ GetChild()->GetContentSize(displaySettings.bounds) };

		switch (axis_)
		{
		case EAxis::Horizontal:
			displaySettings.position.x = displaySettings.position.x + (displaySettings.bounds.x - childSize.x) / 2.0f;
			break;
		case EAxis::Vertical:
			displaySettings.position.y = displaySettings.position.y + (displaySettings.bounds.y - childSize.y) / 2.0f;
			break;
		case EAxis::All:
			displaySettings.position = displaySettings.position + (displaySettings.bounds - childSize) / 2.0f;
			break;
		}
	}
	
	Base::DisplayInternal(displaySettings);
}

#include "generated/Center.generated.inl"
