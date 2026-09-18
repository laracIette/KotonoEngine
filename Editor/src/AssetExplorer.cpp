#include "AssetExplorer.h"

#include "AssetExplorerDirectory.h"
#include "AssetExplorerFile.h"
#include <kotono_interface/widgets.h>
#include <kotono_io/File.h>
#include <kotono_io/FileExplorer.h>

WAssetExplorer::WAssetExplorer()
	: path_{ "${ENGINE_DIRECTORY}" }
	, navigatedPaths_{ path_ }
	, currentPathIndex_{ 0 }
{
}

WidgetPtr WAssetExplorer::Build()
{
	UPtr upButton{ UCreate<WButton>{ "Directory Up Button" }() };
	upButton->SetOnClicked([this]() { Push(path_.Directory()); });

	UPtr previousButton{ UCreate<WButton>{ "Directory Prev Button" }() };
	previousButton->SetOnClicked([this]() { NavigatePrevious(); });

	UPtr nextButton{ UCreate<WButton>{ "Directory Next Button" }() };
	nextButton->SetOnClicked([this]() { NavigateNext(); });

	itemList_ = UCreate<WHorizontalWrapList>{ "Item List" }();
	itemList_->SetItemSpacing(10.0f);
	itemList_->SetRowSpacing(10.0f);
	PopulateItemList();

	selectColor_ = UCreate<WColor>{ "Select Box Color" }(UColor::Mix(Colors::Blue, Colors::Cyan).WithAlpha(0.2f));
	selectColor_->SetIsVisible(false);
	
	auto const widgetTree{ UChildrenOwnerTree{ UCreate<WColumn>{ "Asset Explorer Main Column" }(4.0f), {
		new UChildrenOwnerTree{ UCreate<WRow>{ "Asset Explorer Navigation Row" }(4.0f), {
			new UChildOwnerTree{ UCreate<WWrap>{}(),
				new UChildrenOwnerTree{ UCreate<WStack>{}(), {
					new UWidgetTreeLeaf{ UCreate<WColor>{}(Colors::White.WithValue(0.25f)) },
					new UWidgetTreeLeaf{ upButton },
					new UWidgetTreeLeaf{ UCreate<WText>{ "Directory Up Text" }("Up", glm::vec2{ 16.0f, 20.0f }) },
				} }
			},
			new UChildOwnerTree{ UCreate<WWrap>{}(),
				new UChildrenOwnerTree{ UCreate<WStack>{}(), {
					new UWidgetTreeLeaf{ UCreate<WColor>{}(Colors::White.WithValue(0.25f)) },
					new UWidgetTreeLeaf{ previousButton },
					new UWidgetTreeLeaf{ UCreate<WText>{ "Directory Prev Text" }("Prev", glm::vec2{ 16.0f, 20.0f }) },
				} }
			},
			new UChildOwnerTree{ UCreate<WWrap>{}(),
				new UChildrenOwnerTree{ UCreate<WStack>{}(), {
					new UWidgetTreeLeaf{ UCreate<WColor>{}(Colors::White.WithValue(0.25f)) },
					new UWidgetTreeLeaf{ nextButton },
					new UWidgetTreeLeaf{ UCreate<WText>{ "Directory Next Text" }("Next", glm::vec2{ 16.0f, 20.0f }) },
				} }
			},
		} },
		new UChildrenOwnerTree{ UCreate<WStack>{ "Item List Stack" }(), {
			new UWidgetTreeLeaf{ UCreate<WColor>{ "Item List Background" }(Colors::White.WithValue(0.05f)) },
			new UChildOwnerTree{ UCreate<WPadding>{ "Item List Padding" }(UPadding::All(8.0f)),
				new UWidgetTreeLeaf{ itemList_ }
			},
			new UChildOwnerTree{ selectOffset_ = UCreate<WOffset>{ "Select Offset" }(),
				new UChildOwnerTree{ selectBox_ = UCreate<WBox>{ "Select Box" }(),
					new UWidgetTreeLeaf{ selectColor_ }
				}
			},
		} },
	} } }; 
	widgetTree.Link();

	auto a = WidgetPtr{ widgetTree.Widget() };
	return widgetTree.Widget();
}

b8 WAssetExplorer::OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position)
{
	if (Base::OnMouseButton(button, inputState, position))
	{
		return INPUT_HANDLED;
	}

	switch (button)
	{
	case EButton::Left:
	{
		switch (inputState)
		{
		case EInputState::Pressed:
		{
			auto const offset{ position - selectOffset_->GetPosition() };
			selectOffset_->SetOffset(offset);
			selectColor_->SetIsVisible(true);
			return INPUT_HANDLED;
		}
		case EInputState::Released:
		{
			selectColor_->SetIsVisible(false);
			return INPUT_HANDLED;
		}
		case EInputState::Down:
		{
			auto const size{ position - selectBox_->GetPosition() };
			selectBox_->SetSize(size);
			return INPUT_HANDLED;
		}
		default:
			break;
		}
		break;
	}
	case EButton::Previous:
	{
		if (inputState == EInputState::Pressed)
		{
			NavigatePrevious();
			return INPUT_HANDLED;
		}
		break;
	}
	case EButton::Next:
	{
		if (inputState == EInputState::Pressed)
		{
			NavigateNext();
			return INPUT_HANDLED;
		}
		break;
	}
	default:
		break;
	}

	return INPUT_UNHANDLED;
}

void WAssetExplorer::DeselectOthers(UPtr<WAssetExplorerItem> const& item) const
{
	auto assets{ assetExplorerItems_ 
		| std::views::filter([item](auto const& asset) { return asset != item; })
		| std::views::filter(&UPtr<WAssetExplorerItem>::operator bool)
	};

	for (auto const& asset : assets)
	{
		asset->Deselect();
	}
}

void WAssetExplorer::Push(UPath const& path)
{
	path_ = path;
	++currentPathIndex_;
	navigatedPaths_.erase(navigatedPaths_.begin() + currentPathIndex_, navigatedPaths_.end());
	navigatedPaths_.push_back(path);
	PopulateItemList();
}

void WAssetExplorer::NavigatePrevious()
{
	if (!navigatedPaths_.empty() && currentPathIndex_ > 0)
	{
		path_ = navigatedPaths_[--currentPathIndex_];
		PopulateItemList();
	}
}

void WAssetExplorer::NavigateNext()
{
	if (!navigatedPaths_.empty() && currentPathIndex_ < navigatedPaths_.size() - 1)
	{
		path_ = navigatedPaths_[++currentPathIndex_];
		PopulateItemList();
	}
}

void WAssetExplorer::PopulateItemList()
{
	if (itemList_)
	{
		UAutoDelete<WWidget> const itemListChildren{ itemList_->GetChildren() };

		UFileExplorer const fileExplorer{ path_ };
		auto const directories{ fileExplorer.GetDirectories() };
		auto const files{ fileExplorer.GetFiles() };

		assetExplorerItems_.Clear();
		for (auto const& directory : directories)
		{
			assetExplorerItems_.Add(UCreate<WAssetExplorerDirectory>{}(Ptr(), directory, [this](UPath const& path) { Push(path); }));
		}
		for (auto const& file : files)
		{
			assetExplorerItems_.Add(UCreate<WAssetExplorerFile>{}(Ptr(), file.Path()));
		}
		itemList_->SetChildren(assetExplorerItems_);
	}
}

#include "generated/AssetExplorer.generated.inl"
