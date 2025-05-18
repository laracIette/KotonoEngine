#pragma once
#include "InterfaceComponent.h"
class KInterfaceContainerComponent : public KInterfaceComponent
{
	BASECLASS(KInterfaceComponent)

public:
    void Init() override;
    void Update() override;
    void Cleanup() override;

    const std::vector<KInterfaceComponent*>& GetItems() const;
    const bool GetIsSizeToContent() const;

    void SetIsSizeToContent(const bool isSizeToContent);
    
    void ClearItems();

    virtual void AddItem(KInterfaceComponent* item);

private:
    std::vector<KInterfaceComponent*> items_;
    bool isSizeToContent_;

    void UpdateSize();
};

