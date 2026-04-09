#pragma once
#include "generated/ChildrenOwner.generated.h"
#include "Widget.h"
class WChildrenOwner : public WWidget
{
	GENERATED_WCHILDRENOWNER()

public:
	void CacheBuild() override final;

	void Cleanup() override;

	EFlex GetFlex() const override;

	WidgetVector GetWidgetTree() override;

public:
	const WidgetPool& GetChildren() const;
	void SetChildren(const WidgetPool& widgets);

	void AddChild(const WidgetPtr& widget);
	void RemoveChild(const WidgetPtr& widget);

protected:
	size GetValidChildrenCount() const;

protected:
	WidgetPool children_;
};

