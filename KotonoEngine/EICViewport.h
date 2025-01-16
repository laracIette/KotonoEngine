#pragma once
#include "EngineInterfaceComponent.h"
#include "CameraView.h"
class EICViewport final :
    public EngineInterfaceComponent
{
    BASECLASS(EngineInterfaceComponent)

public:
    EICViewport();

    void OnResize() override;

    void Draw() const override;

private:
    CameraView* _cameraView;
};

