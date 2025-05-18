#include "InterfaceContainerComponent.h"

void KInterfaceContainerComponent::Init()
{
    Base::Init();

    isSizeToContent_ = true;
}

void KInterfaceContainerComponent::Update()
{
	Base::Update();

    if (isSizeToContent_)
    {
        UpdateSize();
    }
}

void KInterfaceContainerComponent::Cleanup()
{
    Base::Cleanup();
}

const std::vector<KInterfaceComponent*>& KInterfaceContainerComponent::GetItems() const
{
    return items_;
}

const bool KInterfaceContainerComponent::GetIsSizeToContent() const
{
    return isSizeToContent_;
}

void KInterfaceContainerComponent::SetIsSizeToContent(const bool isSizeToContent)
{
    isSizeToContent_ = isSizeToContent;
}

void KInterfaceContainerComponent::ClearItems()
{
    items_.clear();
}

void KInterfaceContainerComponent::AddItem(KInterfaceComponent* item)
{
	items_.push_back(item);
}

void KInterfaceContainerComponent::UpdateSize()
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
