#include "InterfaceContainerComponent.h"

void KInterfaceContainerComponent::Init()
{
    Base::Init();
}

void KInterfaceContainerComponent::Update()
{
	Base::Update();
}

void KInterfaceContainerComponent::Cleanup()
{
    Base::Cleanup();
}

const KtPool<KInterfaceComponent*>& KInterfaceContainerComponent::GetItems() const
{
    return items_;
}

bool KInterfaceContainerComponent::GetIsSizeToContent() const
{
    return isSizeToContent_;
}

void KInterfaceContainerComponent::SetIsSizeToContent(const bool isSizeToContent)
{
    isSizeToContent_ = isSizeToContent;
}

void KInterfaceContainerComponent::ClearItems()
{
    items_.Clear();
}

void KInterfaceContainerComponent::AddItem(KInterfaceComponent* item)
{
    items_.Add(item);
    item->GetEventRectUpdated().AddListener(KtDelegate(this, &KInterfaceContainerComponent::TryUpdateSize));
}

void KInterfaceContainerComponent::TryUpdateSize()
{
    if (isSizeToContent_)
    {
        UpdateSize();
    }
}

void KInterfaceContainerComponent::UpdateSize()
{
    float left = INFINITY;
    float right = -INFINITY;
    float top = INFINITY;
    float bottom = -INFINITY;
    for (const auto* item : items_)
    {
        const float itemLeft = item->GetLeft();
        const float itemRight = item->GetRight();
        const float itemTop = item->GetTop();
        const float itemBottom = item->GetBottom();
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

    SetWorldPosition(glm::vec2(left + right, top + bottom) / 2.0f);
    SetWorldSize(glm::vec2(right - left, bottom - top));
}
