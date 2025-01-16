#pragma once
#include "EngineInterfaceComponent.h"
class EICObjectExplorer final :
    public EngineInterfaceComponent
{
    BASECLASS(EngineInterfaceComponent)

public:
    EICObjectExplorer();

    void Draw() const override;

    void OnResize() override;
};

