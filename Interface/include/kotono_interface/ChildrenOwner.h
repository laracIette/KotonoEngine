#pragma once
#include "generated/ChildrenOwner.generated.h"
#include <kotono_core/Widget.h>

#include <concepts>

class WChildrenOwner : public WWidget
{
	GENERATED_WCHILDRENOWNER()

public:
	~WChildrenOwner() override;

	void Remove() override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const final;
	void PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const final;

	b8 OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position) override;
	b8 OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) override;
	b8 OnMouseScroll(glm::vec2 const& delta) override;

	b8 OnKeyboardKey(EKey key, EInputState inputState) override;
	
	void Refresh() final;

	void SetChildren(WidgetSet const& widgets);
	void AddChild(WidgetPtr const& widget);
	void RemoveChild(WidgetPtr const& widget);
	void ReplaceChild(WidgetPtr const& oldWidget, WidgetPtr const& newWidget);

	auto GetChildren() const -> WidgetSet const& { return children_; }

protected:
	size GetValidChildrenCount() const;

private:
	WidgetSet children_;
};

template <typename T>
concept ChildrenOwner = requires(T & widget, WidgetSet const& children)
{
	{ widget.GetChildren() } -> std::convertible_to<WidgetSet>;
	widget.SetChildren(children);
};

template <ChildrenOwner T>
class UChildrenOwnerTree final : public UWidgetTree
{
public:
	UChildrenOwnerTree(UPtr<T> const& widget, std::span<UWidgetTree* const> children)
		: widget_{ widget }
		, children_{ children | std::ranges::to<std::vector>() }
	{}

	UChildrenOwnerTree(UPtr<T> const& widget, std::initializer_list<UWidgetTree*> children)
		: widget_{ widget }
		, children_{ children | std::ranges::to<std::vector>() }
	{}

	~UChildrenOwnerTree() override
	{
		for (auto const* widgetTree : children_)
		{
			delete widgetTree;
		}
	}

	auto Widget() const -> WidgetPtr override
	{
		return widget_;
	}

	void Link() const override
	{
		for (auto const* child : children_)
		{
			if (child)
			{
				child->Link();
			}
		}

		if (widget_)
		{
			auto const widgets{ children_
				| std::views::filter([](UWidgetTree const* child) { return child != nullptr; })
				| std::views::transform([](UWidgetTree const* child) { return child->Widget(); })
				| std::ranges::to<USet>()
			};

			widget_->SetChildren(widgets);
		}
	}

private:
	UPtr<T> widget_;
	std::vector<UWidgetTree*> children_;
};
