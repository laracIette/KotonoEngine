#pragma once
#include "generated/ChildrenOwner.generated.h"
#include "Widget.h"
class WChildrenOwner : public WWidget
{
	GENERATED_WCHILDRENOWNER()

public:
	~WChildrenOwner() override;

	void Remove() override;

	EFlex GetFlex() const override;

	WidgetVector GetWidgetTree() override;

public:
	const WidgetPool& GetChildren() const;
	void SetChildren(const WidgetPool& widgets);

protected:
	size GetValidChildrenCount() const;

protected:
	WidgetPool children_;
};

class UChildrenOwnerTree final : public UWidgetTree
{
public:
	UChildrenOwnerTree(const UPtr<WChildrenOwner>& widget, const std::vector<UWidgetTree*>& children);
	~UChildrenOwnerTree() override;
	
	WidgetPtr Widget() const override;
	void Link() const override;

private:
	UPtr<WChildrenOwner> widget_;
	std::vector<UWidgetTree*> children_;
};
