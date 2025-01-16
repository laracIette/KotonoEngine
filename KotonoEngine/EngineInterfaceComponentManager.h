#pragma once
#include "EngineInterfaceComponent.h"
#include "EngineInterfaceLayout.h"
class EngineInterfaceComponentManager final
{
public:
    EngineInterfaceComponentManager();
    ~EngineInterfaceComponentManager();

    void Update(const float deltaTime);

    void Draw() const;

    EngineInterfaceComponent* GetActiveComponent() const;

private:
    EngineInterfaceComponent* _components[3];

    EngineInterfaceLayout _layout;
};

