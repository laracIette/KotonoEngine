#include "InterfaceObjectStack.h"
#include <kotono_framework/log.h>
#include "Engine.h"
#include "ObjectManager.h"

template <EOrientation Orientation>
const size_t RInterfaceObjectStack<Orientation>::GetItemCount() const
{
    return _placeholders.size();
}

template <EOrientation Orientation>
const float RInterfaceObjectStack<Orientation>::GetItemSpacing() const
{
    return _spacing;
}

template <EOrientation Orientation>
void RInterfaceObjectStack<Orientation>::SetItemSpacing(const float spacing)
{
    _spacing = spacing;
    UpdatePositions();
}

template <EOrientation Orientation>
void RInterfaceObjectStack<Orientation>::AddItem(RInterfaceObject* item)
{
    auto* placeholder = Engine.GetObjectManager().Create<RInterfaceObject>();
    _placeholders.push_back(placeholder);
    item->SetParent(placeholder, ECoordinateSpace::Relative);
    UpdatePositions();
}

template <EOrientation Orientation>
void RInterfaceObjectStack<Orientation>::UpdatePositions()
{
    for (size_t i = 0; i < GetItemCount(); i++)
    {
        auto* placeholder = _placeholders[i];
        const float offset = _spacing * i;

        if constexpr (Orientation == EOrientation::Horizontal)
        {
            placeholder->GetRect().SetRelativePosition(glm::vec2(offset, 0.0f));
        }
        else if constexpr (Orientation == EOrientation::Vertical)
        {
            placeholder->GetRect().SetRelativePosition(glm::vec2(0.0f, offset));
        }
    }
}



template const size_t RHorizontalInterfaceObjectStack::GetItemCount() const;
template const size_t RVerticalInterfaceObjectStack::GetItemCount() const;
 
template const float RHorizontalInterfaceObjectStack::GetItemSpacing() const;
template const float RVerticalInterfaceObjectStack::GetItemSpacing() const;
 
template void RHorizontalInterfaceObjectStack::SetItemSpacing(const float spacing);
template void RVerticalInterfaceObjectStack::SetItemSpacing(const float spacing);
 
template void RHorizontalInterfaceObjectStack::AddItem(RInterfaceObject* item);
template void RVerticalInterfaceObjectStack::AddItem(RInterfaceObject* item);
 
template void RHorizontalInterfaceObjectStack::UpdatePositions();
template void RVerticalInterfaceObjectStack::UpdatePositions();