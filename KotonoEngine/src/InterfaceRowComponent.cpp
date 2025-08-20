#include "InterfaceRowComponent.h"
#include "log.h"

void KInterfaceRowComponent::Init()
{
    Base::Init();

    SetIsSizeToContent(true);
}

float KInterfaceRowComponent::GetSpacing() const
{
    return spacing_;
}

void KInterfaceRowComponent::SetSpacing(const float spacing)
{
    spacing_ = spacing;
    UpdatePositions();
}

void KInterfaceRowComponent::AddItem(KInterfaceComponent* item)
{
    Base::AddItem(item);
    item->GetEventRectUpdated().AddListener(KtDelegate(this, &KInterfaceRowComponent::UpdatePositions));
    UpdatePositions();
}

void KInterfaceRowComponent::UpdatePositions()
{
    float offset{ 0.0f };
    for (auto* item : GetItems())
    {
        item->SetRelativePosition({ offset, 0.0f });
        offset += spacing_ + 0.1f;
        KT_LOG_KE(KT_LOG_COMPILE_TIME_LEVEL, "%f", offset);
    }
}
