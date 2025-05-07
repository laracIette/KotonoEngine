#include "InterfaceObjectStack.h"

template<EOrientation Orientation>
void RInterfaceObjectStack<Orientation>::Update()
{
    Base::Update();
    //SetIsSizeToContent(false);
}

template <EOrientation Orientation>
const float RInterfaceObjectStack<Orientation>::GetItemSpacing() const
{
    return spacing_;
}

template <EOrientation Orientation>
void RInterfaceObjectStack<Orientation>::SetItemSpacing(const float spacing)
{
    spacing_ = spacing;
    UpdatePositions();
}

template <EOrientation Orientation>
void RInterfaceObjectStack<Orientation>::AddItem(RInterfaceObject* item)
{
    Base::AddItem(item);
    UpdatePositions();
}

template <EOrientation Orientation>
void RInterfaceObjectStack<Orientation>::UpdatePositions()
{
    const auto& items = GetItems();
    for (size_t i = 0; i < items.size(); i++)
    {
        auto* item = items[i];
        const float offset = spacing_ * i;

        if constexpr (Orientation == EOrientation::Horizontal)
        {
            item->GetRect().SetRelativePosition(glm::vec2(offset, 0.0f));
        }
        else if constexpr (Orientation == EOrientation::Vertical)
        {
            item->GetRect().SetRelativePosition(glm::vec2(0.0f, offset));
        }
    }
}



template void RHorizontalInterfaceObjectStack::Update();
template void RVerticalInterfaceObjectStack::Update();
 
template const float RHorizontalInterfaceObjectStack::GetItemSpacing() const;
template const float RVerticalInterfaceObjectStack::GetItemSpacing() const;
 
template void RHorizontalInterfaceObjectStack::SetItemSpacing(const float spacing);
template void RVerticalInterfaceObjectStack::SetItemSpacing(const float spacing);
 
template void RHorizontalInterfaceObjectStack::AddItem(RInterfaceObject* item);
template void RVerticalInterfaceObjectStack::AddItem(RInterfaceObject* item);
 
template void RHorizontalInterfaceObjectStack::UpdatePositions();
template void RVerticalInterfaceObjectStack::UpdatePositions();