#pragma once
#include "InterfaceObjectContainer.h"
#include "Orientation.h"
template <EOrientation Orientation>
class RInterfaceObjectStack : public RInterfaceObjectContainer
{
    BASECLASS(RInterfaceObjectContainer)

public:
    void Init() override;

    const float GetItemSpacing() const;

    void SetItemSpacing(const float spacing);

    void AddItem(RInterfaceObject* item) override;

private:
    float spacing_;

    void UpdatePositions();
};

typedef RInterfaceObjectStack<EOrientation::Horizontal> RHorizontalInterfaceObjectStack;
typedef RInterfaceObjectStack<EOrientation::Vertical> RVerticalInterfaceObjectStack;