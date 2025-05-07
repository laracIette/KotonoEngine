#include "InterfaceObjectStack.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Window.h>
#include <kotono_framework/log.h>
#include "Engine.h"
#include "ObjectManager.h"

template <EOrientation Orientation>
const size_t RInterfaceObjectStack<Orientation>::GetItemCount() const
{
    return placeholders_.size();
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
    auto* placeholder = Engine.GetObjectManager().Create<RInterfaceObject>();
    placeholders_.push_back(placeholder);
    item->SetParent(placeholder, ECoordinateSpace::Relative);
    UpdatePositions();

    const float worldSize = GetItemCount() * spacing_;
    const float screenSize = worldSize * Framework.GetWindow().GetSize().x / 2.0f;
    GetRect().SetSize(glm::vec2(screenSize, 100.0f));
}

template <EOrientation Orientation>
void RInterfaceObjectStack<Orientation>::UpdatePositions()
{
    for (size_t i = 0; i < GetItemCount(); i++)
    {
        auto* placeholder = placeholders_[i];
        const float offset = spacing_ * i;

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