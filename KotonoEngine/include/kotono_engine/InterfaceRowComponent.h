#pragma once
#include "InterfaceContainerComponent.h"
class KInterfaceRowComponent final : public KInterfaceContainerComponent
{
    BASECLASS(KInterfaceContainerComponent)

protected:
    void Init() override;

public:
    float GetSpacing() const;

    void SetSpacing(float spacing);

    void AddItem(KInterfaceComponent* item) override;

private:
    float spacing_;

    void UpdatePositions();
};