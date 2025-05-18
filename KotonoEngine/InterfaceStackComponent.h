#pragma once
#include "InterfaceContainerComponent.h"
#include "Orientation.h"
class KInterfaceStackComponent : public KInterfaceContainerComponent
{
    BASECLASS(KInterfaceContainerComponent)

public:
    void Init() override;

    const EOrientation GetOrientation() const;
    const float GetItemSpacing() const;

    void SetOrientation(const EOrientation orientation);
    void SetItemSpacing(const float spacing);

    void AddItem(KInterfaceComponent* item) override;

private:
    EOrientation orientation_;
    float spacing_;

    void UpdatePositions();
};