#pragma once
#include "InterfaceObject.h"
#include "Orientation.h"
#include <vector>
template <EOrientation Orientation>
class RInterfaceObjectStack : public RInterfaceObject
{
    BASECLASS(RInterfaceObject)

public:
    const size_t GetItemCount() const;
    const float GetItemSpacing() const;

    void SetItemSpacing(const float spacing);

    void AddItem(RInterfaceObject* item);

private:
    std::vector<RInterfaceObject*> placeholders_;
    float spacing_;

    void UpdatePositions();
};

typedef RInterfaceObjectStack<EOrientation::Horizontal> RHorizontalInterfaceObjectStack;
typedef RInterfaceObjectStack<EOrientation::Vertical> RVerticalInterfaceObjectStack;