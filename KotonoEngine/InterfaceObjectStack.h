#pragma once
#include "InterfaceObjectContainer.h"
#include "Orientation.h"
class RInterfaceObjectStack : public RInterfaceObjectContainer
{
    BASECLASS(RInterfaceObjectContainer)

public:
    void Init() override;

    const EOrientation GetOrientation() const;
    const float GetItemSpacing() const;

    void SetOrientation(const EOrientation orientation);
    void SetItemSpacing(const float spacing);

    void AddItem(RInterfaceObject* item) override;

private:
    EOrientation orientation_;
    float spacing_;

    void UpdatePositions();
};