#pragma once
#include "generated/ChildOwner.generated.h"
#include "Widget.h"
class WChildOwner : public WWidget
{
	GENERATED_WCHILDOWNER()

public:
	~WChildOwner() override;

	void Remove() override;

	EFlex GetFlex() const override;
	glm::vec2 GetDesiredSize(glm::vec2 bounds) const override;

	WidgetVector GetWidgetTree() override;

public:
	const WidgetPtr& GetChild() const;

	void SetChild(const WidgetPtr& widget);

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) final;

protected:
	WidgetPtr child_;
};

class UChildOwnerTree final : public UWidgetTree
{
public:
	UChildOwnerTree(const UPtr<WChildOwner>& widget, UWidgetTree* child);
	~UChildOwnerTree() override;

	WidgetPtr Widget() const override;
	void Link() const override;

private:
	UPtr<WChildOwner> widget_;
	UWidgetTree* child_;
};
