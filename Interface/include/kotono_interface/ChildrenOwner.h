#pragma once
#include "generated/ChildrenOwner.generated.h"
#include <kotono_object/Widget.h>
class WChildrenOwner : public WWidget
{
	GENERATED_WCHILDRENOWNER()

public:
	~WChildrenOwner() override;

	void Remove() override;

	WidgetVector WidgetTree() const override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

	void SetChildren(WidgetSet const& widgets);

protected:
	size GetValidChildrenCount() const;

private:
	ReadonlyProperty(WidgetSet, children_, Children);
};

class UChildrenOwnerTree final : public UWidgetTree
{
public:
	UChildrenOwnerTree(UPtr<WChildrenOwner> const& widget, std::span<UWidgetTree* const> children);
	UChildrenOwnerTree(UPtr<WChildrenOwner> const& widget, std::initializer_list<UWidgetTree*> children);
	~UChildrenOwnerTree() override;
	
	WidgetPtr Widget() const override;
	void Link() const override;

private:
	UPtr<WChildrenOwner> widget_;
	std::vector<UWidgetTree*> children_;
};
