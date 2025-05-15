#include "InterfaceObjectStack.h"

void RInterfaceObjectStack::Init()
{
    Base::Init();
    //SetIsSizeToContent(false);
}

const EOrientation RInterfaceObjectStack::GetOrientation() const
{
    return orientation_;
}

const float RInterfaceObjectStack::GetItemSpacing() const
{
    return spacing_;
}

void RInterfaceObjectStack::SetOrientation(const EOrientation orientation)
{
    orientation_ = orientation;
}

void RInterfaceObjectStack::SetItemSpacing(const float spacing)
{
    spacing_ = spacing;
    UpdatePositions();
}

void RInterfaceObjectStack::AddItem(RInterfaceObject* item)
{
    Base::AddItem(item);
    UpdatePositions();
}

void RInterfaceObjectStack::UpdatePositions()
{
    const auto& items = GetItems();
    for (size_t i = 0; i < items.size(); i++)
    {
        auto* item = items[i];
        const float offset = spacing_ * i;

        if (orientation_ == EOrientation::Horizontal)
        {
            item->GetRect().SetRelativePosition(glm::vec2(offset, 0.0f));
        }
        else if (orientation_ == EOrientation::Vertical)
        {
            item->GetRect().SetRelativePosition(glm::vec2(0.0f, offset));
        }
    }
}
