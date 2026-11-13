#pragma once
#include "Widget/Widget.h"

#include "WidgetContainer.generated.h"

ABSTRACT class WWidgetContainer : public WWidget
{
    GENERATED()

public:
    virtual auto GetChildren() const -> WidgetSet const& = 0;
    virtual void SetChildren(WidgetSet const& children) = 0;
    virtual void AddChild(WidgetPtr const& child) = 0;
};
