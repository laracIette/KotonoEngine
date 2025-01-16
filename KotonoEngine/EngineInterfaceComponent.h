#pragma once
#include "Updatable.h"
#include "EngineInterfaceRect.h"
class EngineInterfaceComponent :
    public Updatable
{
    BASECLASS(Updatable)

public:
    EngineInterfaceComponent(const std::string& name);

    const bool GetIsVisible() const;
    const bool GetIsActive() const;

    EngineInterfaceRect& GetRect();

    void SetIsVisible(const bool isVisible);
    void SetIsActive(const bool isActive);

    virtual void Draw() const = 0;

    virtual void OnResize() = 0;

private:
    bool _isVisible;
    bool _isActive;

    EngineInterfaceRect _rect;
};

