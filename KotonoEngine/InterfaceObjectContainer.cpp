#include "InterfaceObjectContainer.h"

void RInterfaceObjectContainer::Init()
{
    Base::Init();

    isSizeToContent_ = true;
}

void RInterfaceObjectContainer::Update()
{
	Base::Update();

    if (isSizeToContent_)
    {
        UpdateSize();
    }
}

void RInterfaceObjectContainer::Cleanup()
{
    Base::Cleanup();
}

const std::vector<RInterfaceObject*>& RInterfaceObjectContainer::GetItems() const
{
	return items_;
}

const bool RInterfaceObjectContainer::GetIsSizeToContent() const
{
    return isSizeToContent_;
}

void RInterfaceObjectContainer::SetIsSizeToContent(const bool isSizeToContent)
{
    isSizeToContent_ = isSizeToContent;
}

void RInterfaceObjectContainer::ClearItems()
{
    items_.clear();
}

void RInterfaceObjectContainer::AddItem(RInterfaceObject* item)
{
	items_.push_back(item);
}

void RInterfaceObjectContainer::UpdateSize()
{
    float left = INFINITY;
    float right = -INFINITY;
    float top = INFINITY;
    float bottom = -INFINITY;
    for (const auto* item : items_)
    {
        const float itemLeft = item->GetRect().GetLeft();
        const float itemRight = item->GetRect().GetRight();
        const float itemTop = item->GetRect().GetTop();
        const float itemBottom = item->GetRect().GetBottom();
        if (itemLeft < left)
        {
            left = itemLeft;
        }
        if (itemRight > right)
        {
            right = itemRight;
        }
        if (itemTop < top)
        {
            top = itemTop;
        }
        if (itemBottom > bottom)
        {
            bottom = itemBottom;
        }
    }

    GetRect().SetWorldPosition(glm::vec2(left + right, top + bottom) / 2.0f);
    GetRect().SetWorldSize(glm::vec2(right - left, bottom - top));
}
