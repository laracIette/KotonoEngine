#include "AssetExplorer.h"

#include "AssetExplorerDirectory.h"
#include "AssetExplorerFile.h"
#include <kotono_input/Mouse.h>
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
	UPtr upText{ UCreate<WText>{ "Directory Up Text" }()};
	upText->SetText("Up");
	upText->SetFontSize({ 16.0f, 20.0f });

	UPtr upButton{ UCreate<WButton>{ "Directory Up Button" }() };
	upButton->SetOnClicked([this]() { Push(path_.Directory()); });


	UPtr previousText{ UCreate<WText>{ "Directory Prev Text" }() };
	previousText->SetText("Prev");
	previousText->SetFontSize({ 16.0f, 20.0f });

	UPtr previousButton{ UCreate<WButton>{ "Directory Prev Button" }() };
	previousButton->SetOnClicked([this]() { NavigatePrevious(); });


	UPtr nextText{ UCreate<WText>{ "Directory Next Text" }() };
	nextText->SetText("Next");
	nextText->SetFontSize({ 16.0f, 20.0f });

	UPtr nextButton{ UCreate<WButton>{ "Directory Next Button" }() };
	nextButton->SetOnClicked([this]() { NavigateNext(); });


	itemList_ = UCreate<WHorizontalWrapList>{ "Item List" }();
	itemList_->SetItemSpacing(10.0f);
	itemList_->SetRowSpacing(10.0f);
	PopulateItemList();

	auto const widgetTree{ UChildrenOwnerTree{ UCreate<WColumn>{ "Asset Explorer Main Column" }(4.0f), {
		new UChildrenOwnerTree{ UCreate<WRow>{ "Asset Explorer Navigation Row" }(4.0f), {
			new UChildOwnerTree{ UCreate<WWrap>{}(),
				new UChildrenOwnerTree{ UCreate<WStack>{}(), {
					new UWidgetTreeLeaf{ UCreate<WColor>{}(Colors::White.WithValue(0.25f)) },
					new UWidgetTreeLeaf{ upText },
					new UWidgetTreeLeaf{ upButton },
				} }
			},
			new UChildOwnerTree{ UCreate<WWrap>{}(),
				new UChildrenOwnerTree{ UCreate<WStack>{}(), {
					new UWidgetTreeLeaf{ UCreate<WColor>{}(Colors::White.WithValue(0.25f)) },
					new UWidgetTreeLeaf{ previousText },
					new UWidgetTreeLeaf{ previousButton },
				} }
			},
			new UChildOwnerTree{ UCreate<WWrap>{}(),
				new UChildrenOwnerTree{ UCreate<WStack>{}(), {
					new UWidgetTreeLeaf{ UCreate<WColor>{}(Colors::White.WithValue(0.25f)) },
					new UWidgetTreeLeaf{ nextText },
					new UWidgetTreeLeaf{ nextButton },
				} }
			},
		} },
		new UChildrenOwnerTree{ UCreate<WStack>{ "Item List Stack" }(), {
			new UWidgetTreeLeaf{ UCreate<WColor>{ "Item List Background" }(Colors::White.WithValue(0.5f).WithAlpha(0.4f))},
			new UChildOwnerTree{ UCreate<WPadding>{ "Item List Padding" }(UPadding::All(8.0f)),
				new UWidgetTreeLeaf{ itemList_ }
			},
		} },
	} } }; 
	widgetTree.Link();

	return widgetTree.Widget();
}

void WAssetExplorer::Display(UWidgetDisplaySettings const& displaySettings)
{
	Base::Display(displaySettings);

	Mouse.EventButton(EButton::Previous, EInputState::Pressed).AddListener(this, &WAssetExplorer::OnMousePreviousButtonPressed);
	Mouse.EventButton(EButton::Next, EInputState::Pressed).AddListener(this, &WAssetExplorer::OnMouseNextButtonPressed);
}

void WAssetExplorer::Remove()
{
	Base::Remove();

	Mouse.EventButton(EButton::Previous, EInputState::Pressed).RemoveListener(this, &WAssetExplorer::OnMousePreviousButtonPressed);
	Mouse.EventButton(EButton::Next, EInputState::Pressed).RemoveListener(this, &WAssetExplorer::OnMouseNextButtonPressed);
}

void WAssetExplorer::Push(const UPath& path)
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

		WidgetSet assets{};
		for (auto const& directory : directories)
		{
			assets.Add(UCreate<WAssetExplorerDirectory>{}(directory, [this](const UPath& path) { Push(path); }));
		}
		for (auto const& file : files)
		{
			assets.Add(UCreate<WAssetExplorerFile>{}(file.Path()));
		}
		itemList_->SetChildren(assets);
	}
}

void WAssetExplorer::OnMousePreviousButtonPressed()
{
	NavigatePrevious();
}

void WAssetExplorer::OnMouseNextButtonPressed()
{
	NavigateNext();
}

#include "generated/AssetExplorer.generated.inl"
