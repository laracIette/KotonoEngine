#pragma once
#include "InterfaceObject.h"
class RInterfaceObjectContainer : public RInterfaceObject
{
	BASECLASS(RInterfaceObject)

public:
    void Init() override;
    void Update() override;

    const std::vector<RInterfaceObject*>& GetItems() const;
    const bool GetIsSizeToContent() const;

    void SetIsSizeToContent(const bool isSizeToContent);

    virtual void AddItem(RInterfaceObject* item);

private:
    std::vector<RInterfaceObject*> items_;
    bool isSizeToContent_;

    void UpdateSize();
};

