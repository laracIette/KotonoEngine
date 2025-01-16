#pragma once
#include "Object.h"
#include "EngineInterfaceElementRect.h"
class EngineInterfaceElement :
    public Object
{
    BASECLASS(Object)

public:
    const int GetLayer() const;

    const bool IsHovered() const;

    virtual void OnClicked() = 0;

    virtual void Draw() = 0;

private:
    EngineInterfaceElement* _parent;

    EngineInterfaceElementRect _localRect;
};

