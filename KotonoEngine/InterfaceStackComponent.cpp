#include "InterfaceStackComponent.h"

void KInterfaceStackComponent::Init()
{
    Base::Init();
    //SetIsSizeToContent(false);
}

const EOrientation KInterfaceStackComponent::GetOrientation() const
{
    return orientation_;
}

const float KInterfaceStackComponent::GetItemSpacing() const
{
    return spacing_;
}

void KInterfaceStackComponent::SetOrientation(const EOrientation orientation)
{
    orientation_ = orientation;
}

void KInterfaceStackComponent::SetItemSpacing(const float spacing)
{
    spacing_ = spacing;
    UpdatePositions();
}

void KInterfaceStackComponent::AddItem(KInterfaceComponent* item)
{
    Base::AddItem(item);
    UpdatePositions();
}

void KInterfaceStackComponent::UpdatePositions()
{
    const auto& items = GetItems();
    for (size_t i = 0; i < items.size(); i++)
    {
        auto* item = items[i];
        const float offset = spacing_ * i;

        if (orientation_ == EOrientation::Horizontal)
        {
            item->SetRelativePosition(glm::vec2(offset, 0.0f));
        }
        else if (orientation_ == EOrientation::Vertical)
        {
            item->SetRelativePosition(glm::vec2(0.0f, offset));
        }
    }
}
